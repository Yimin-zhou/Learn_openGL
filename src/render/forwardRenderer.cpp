#include <iostream>

#include "forwardRenderer.h"
#include "util/config.h"

#include <glm/gtx/string_cast.hpp>

ForwardRenderer::ForwardRenderer() :
	m_camera(Camera(glm::vec3(0.0f, 1.0f, 2.0f), -90.0f, 0.0f, 3.0f, 0.05f)),
	m_aspectRatio(16.0f / 9.0f), m_finalFrambufferWidth(800), m_finalFrambufferHeight(600),
	m_finalFramebuffer(0), m_finalTexture(0), m_depthBuffer(0), m_shaderManager(ShaderManager()), m_lightManager(LightManager()),
	m_renderSize(800, 600)
{
}

void ForwardRenderer::init()
{
	m_models.push_back(Model(modelPath, ModelType::OPAQUE));

	m_shaderManager.buildShader(ShaderName::PBR, "shader/pbr/pbr_vert.glsl", "shader/pbr/pbr_frag.glsl");
	m_shaderManager.buildShader(ShaderName::LAMBERT, "shader/lambert_vert.glsl", "shader/lambert_frag.glsl");
	m_shaderManager.buildShader(ShaderName::SKYBOX, "shader/skybox/skybox_vert.glsl", "shader/skybox/skybox_frag.glsl");
	m_shaderManager.buildComputeShader(ShaderName::CONVOLUTION, "shader/pbr/convolve_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PREFILTER, "shader/pbr/prefilter_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PRE_BRDF, "shader/pbr/precompute_brdf.glsl");

	// light
	{
		std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f);
		directionalLight->setDirection({ 0.0f, 1.0f, 1.0f });

		m_lightManager.addLight(directionalLight);
		//m_lightManager.addLight(std::make_shared<PointLight>(glm::vec3(1.0f, 2.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, 1.0f, 0.09f, 0.032f, 20.0f));
		//m_lightManager.addLight(std::make_shared<PointLight>(glm::vec3(-1.0f, 2.0f, 1.0f), glm::vec3(0.0f, 0.8f, 1.0f), 30.0f, 1.0f, 0.09f, 0.032f, 20.0f));
	}

	// create framebuffer
	createFinalBuffer();
}

void ForwardRenderer::preprocess()
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

void ForwardRenderer::update(std::shared_ptr<Window> window, float deltaTime)
{
	m_camera.update(window, deltaTime);
}

void ForwardRenderer::draw(glm::vec2 renderSize)
{
	// render pass
	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_finalFramebuffer);
	glClearColor(0.21f, 0.22f, 0.247f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, renderSize.x, renderSize.y);

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
				material.setShader(m_shaderManager.getShader(material.getShaderName()));
				material.use(model.getModelMatrix(), m_camera.getProjectionMatrix() * m_camera.getViewMatrix(), m_camera.getPosition());
				material.useEnvironmentMap(m_skyBox.getCubemapTextureID());
				material.useIrradianceMap(m_skyBox.getIrradianceMapID());
				material.usePrefilterMap(m_skyBox.getPrefilterMapID());
				material.useBRDFLUTTexture(m_skyBox.getBRDFLUTTexture());
				m_lightManager.updateShader(material.getShader());

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

	// draw skybox
	m_skyBox.draw(glm::mat4(glm::mat3(m_camera.getViewMatrix())), m_camera.getProjectionMatrix(), m_shaderManager.getShader(ShaderName::SKYBOX));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ForwardRenderer::resize(glm::vec2 renderSize)
{
	// when window is resized, we need to resize the framebuffer
	m_renderSize = renderSize;
	createFinalBuffer();
}

void ForwardRenderer::setCameraAspectRatio(float aspectRatio)
{
	m_camera.setAspectRatio(aspectRatio);
}

void ForwardRenderer::createFinalBuffer()
{
	// init final framebuffer texture
	glGenTextures(1, &m_finalTexture);
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderSize.x, m_renderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification filter
	glBindTexture(GL_TEXTURE_2D, 0);

	// init final framebuffer
	glGenFramebuffers(1, &m_finalFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_finalFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_finalTexture, 0); // attach texture to framebuffer

	// init depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_renderSize.x, m_renderSize.y);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer); // attach depth buffer to framebuffer

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
