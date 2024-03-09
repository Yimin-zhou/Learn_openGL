#include <iostream>

#include "DeferredRenderer.h"
#include "util/config.h"

#include <glm/gtx/string_cast.hpp>

DeferredRenderer::DeferredRenderer() :
	m_camera(Camera(glm::vec3(0.0f, 1.0f, 2.0f), -90.0f, 0.0f, 3.0f, 0.05f)),
	m_aspectRatio(16.0f / 9.0f), m_finalFrambufferWidth(800), m_finalFrambufferHeight(600),
	m_finalFramebuffer(0), m_finalTexture(0), m_depthStencilBuffer(0), m_shaderManager(ShaderManager()), m_lightManager(LightManager()),
	m_skyBox(SkyBox()), m_gBuffer(0), m_gWorldPositionTex(0), m_gWorldNormalTex(0), m_gAlbedoTex(0), m_gRoughnessMetalnessAoTex(0), m_gEmissionTex(0)
{
}

void DeferredRenderer::init()
{
	m_models.push_back(Model(modelPath, ModelType::OPAQUE));

	m_shaderManager.buildShader(ShaderName::GBUFFER, "shader/deferred/geo_vert.glsl", "shader/deferred/geo_frag.glsl");
	m_shaderManager.buildShader(ShaderName::PBR, "shader/pbr/pbr_vert.glsl", "shader/pbr/pbr_frag.glsl");
	m_shaderManager.buildShader(ShaderName::SKYBOX, "shader/skybox/skybox_vert.glsl", "shader/skybox/skybox_frag.glsl");
	m_shaderManager.buildComputeShader(ShaderName::CONVOLUTION, "shader/pbr/convolve_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PREFILTER, "shader/pbr/prefilter_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PRE_BRDF, "shader/pbr/precompute_brdf.glsl");

	// light
	{
		std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f);
		directionalLight->setDirection({ 0.0f, 1.0f, 1.0f });

		m_lightManager.addLight(directionalLight);
	}
}

void DeferredRenderer::preprocess()
{
	// skybox
	m_skyBox.initData(std::vector<std::filesystem::path>{
		"res/textures/skybox/pure/px.png",
			"res/textures/skybox/pure/nx.png",
			"res/textures/skybox/pure/py.png",
			"res/textures/skybox/pure/ny.png",
			"res/textures/skybox/pure/pz.png",
			"res/textures/skybox/pure/nz.png"
	});
	m_skyBox.convolveCubemap(m_shaderManager.getShader(ShaderName::CONVOLUTION));
	m_skyBox.prefilterCubemap(m_shaderManager.getShader(ShaderName::PREFILTER));
	m_skyBox.generateBRDFLUTTexture(m_shaderManager.getShader(ShaderName::PRE_BRDF));
}

void DeferredRenderer::update(std::shared_ptr<Window> window, float deltaTime)
{
	m_camera.update(window, deltaTime);
}

void DeferredRenderer::draw(glm::vec2 renderSize)
{
	if (m_renderSize != renderSize) m_renderSize = renderSize;
	// geometry pass
	geometryPass();


	// draw skybox
	m_skyBox.draw(glm::mat4(glm::mat3(m_camera.getViewMatrix())), m_camera.getProjectionMatrix(), m_shaderManager.getShader(ShaderName::SKYBOX));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::resize(glm::vec2 renderSize)
{
	// when window is resized, we need to resize the framebuffers
	m_renderSize = renderSize;

	createGbuffer();
}

void DeferredRenderer::setCameraAspectRatio(float aspectRatio)
{
	m_camera.setAspectRatio(aspectRatio);
}

void DeferredRenderer::createGbuffer()
{
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	// position color buffer
	glGenTextures(1, &m_gWorldPositionTex);
	glBindTexture(GL_TEXTURE_2D, m_gWorldPositionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_renderSize.x, m_renderSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gWorldPositionTex, 0);

	// normal color buffer
	glGenTextures(1, &m_gWorldNormalTex);
	glBindTexture(GL_TEXTURE_2D, m_gWorldNormalTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_renderSize.x, m_renderSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gWorldNormalTex, 0);

	// albedo color buffer
	glGenTextures(1, &m_gAlbedoTex);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderSize.x, m_renderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoTex, 0);

	// roughness, metallic, ao color buffer
	glGenTextures(1, &m_gRoughnessMetalnessAoTex);
	glBindTexture(GL_TEXTURE_2D, m_gRoughnessMetalnessAoTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_renderSize.x, m_renderSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gRoughnessMetalnessAoTex, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// depth stencil buffer
	glGenRenderbuffers(1, &m_depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_renderSize.x, m_renderSize.y);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::geometryPass()
{
	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_renderSize.x, m_renderSize.y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	for (auto& model : m_models)
	{
		for (const auto& mesh : model.getMeshes())
		{
			// draw mesh
			glBindVertexArray(mesh.getVao());
			for (const auto& entry : mesh.getEntries())
			{
				// draw submesh
				if (entry.materialIndex < 0)
				{
					// TODO: use default material
				}

				Material material = model.getMaterials()[entry.materialIndex];
				material.setShader(m_shaderManager.getShader(ShaderName::GBUFFER));
				material.use(model.getModelMatrix(), m_camera.getProjectionMatrix() * m_camera.getViewMatrix(), m_camera.getPosition());

				glDrawElementsBaseVertex(GL_TRIANGLES,
					entry.numIndices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(uint32_t) * entry.baseIndex),
					entry.baseVertex);

				material.ubind();
			}
			glBindVertexArray(0);
		}
	}
}
