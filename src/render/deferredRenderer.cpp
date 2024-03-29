#include <iostream>

#include "DeferredRenderer.h"
#include "util/config.h"

#include <glm/gtx/string_cast.hpp>

DeferredRenderer::DeferredRenderer(std::shared_ptr<Scene> scene) :
	m_aspectRatio(16.0f / 9.0f),
	m_lightingFBO(0), m_lightingTexture(0),
	m_gBuffer(),
	m_renderSize(800, 600),
	m_scene(scene)
{
}

void DeferredRenderer::init()
{
	// init scene
	m_scene->init();

	// create framebuffers
	createGbuffer();
	createLightingFramebuffer();
}

void DeferredRenderer::preprocess()
{
	// preprocess scene
	m_scene->preprocess();

	// shadow
	m_shadow = Shadow(2048, 2048);
}

void DeferredRenderer::update(std::shared_ptr<Window> window, float deltaTime)
{
	m_scene->update(window, deltaTime);
}

void DeferredRenderer::draw(glm::vec2 renderSize)
{
	if (m_renderSize != renderSize) m_renderSize = renderSize;
	// shadow pass
	shadowPass();

	// geometry pass
	geometryPass();

	// lighting pass on quad
	lightingPass();	

	m_scene->getSkyBox().draw(glm::mat4(glm::mat3(m_scene->getCamera().getViewMatrix())), 
		m_scene->getCamera().getProjectionMatrix(), m_scene->getShaderManager().getShader(ShaderName::SKYBOX));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::resize(glm::vec2 renderSize)
{
	// when window is resized, we need to resize the framebuffers
	m_renderSize = renderSize;

	createGbuffer();
	createLightingFramebuffer();
}

void DeferredRenderer::setCameraAspectRatio(float aspectRatio)
{
	m_scene->getCamera().setAspectRatio(aspectRatio);
}

void DeferredRenderer::createGbuffer()
{
	glGenFramebuffers(1, &m_gBuffer.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer.fbo);

	// position color buffer
	glGenTextures(1, &m_gBuffer.worldPosTex);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.worldPosTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_renderSize.x, m_renderSize.y, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBuffer.worldPosTex, 0);

	// normal color buffer
	glGenTextures(1, &m_gBuffer.worldNormalTex);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.worldNormalTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_renderSize.x, m_renderSize.y, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBuffer.worldNormalTex, 0);

	// albedo color buffer
	glGenTextures(1, &m_gBuffer.albedoTex);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.albedoTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_renderSize.x, m_renderSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBuffer.albedoTex, 0);

	// roughness, metallic, ao color buffer
	glGenTextures(1, &m_gBuffer.roughnessMetalnessAoTex);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.roughnessMetalnessAoTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_renderSize.x, m_renderSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gBuffer.roughnessMetalnessAoTex, 0);

	// emission color buffer
	glGenTextures(1, &m_gBuffer.emissionTex);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.emissionTex);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_renderSize.x, m_renderSize.y, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_gBuffer.emissionTex, 0);

	// Create depth-stencil texture
	glGenTextures(1, &m_gBuffer.depthStencilTex);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.depthStencilTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_renderSize.x, m_renderSize.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	// Set texture parameters as necessary
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_gBuffer.depthStencilTex, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::createLightingFramebuffer()
{
	glGenTextures(1, &m_lightingTexture);
	glBindTexture(GL_TEXTURE_2D, m_lightingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderSize.x, m_renderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification filter
	glBindTexture(GL_TEXTURE_2D, 0);

	// init light framebuffer
	glGenFramebuffers(1, &m_lightingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightingTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_gBuffer.depthStencilTex, 0);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// create quad for lighting pass
	float quadVertices[] = 
	{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

void DeferredRenderer::shadowPass()
{
	m_shadow.renderShadowMap(m_scene->getModels(), m_scene->getShaderManager().getShader(ShaderName::SHADOW), 
		m_scene->getLightManager().getDirectionalLight()->getDirection());
}

void DeferredRenderer::geometryPass()
{
	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, m_renderSize.x, m_renderSize.y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	for (auto& model : m_scene->getModels())
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
				material.setShader(m_scene->getShaderManager().getShader(ShaderName::GBUFFER));
				material.useGeometryPass(model.getModelMatrix(), 
					m_scene->getCamera().getProjectionMatrix() * m_scene->getCamera().getViewMatrix());

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

void DeferredRenderer::lightingPass()
{
	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightingFBO);
	glViewport(0, 0, m_renderSize.x, m_renderSize.y);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	std::shared_ptr<Shader> shader = m_scene->getShaderManager().getShader(ShaderName::PBR);
	shader->bind();

	shader->setUniform("cameraPos", m_scene->getCamera().getPosition());

	// bind gbuffer textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.worldPosTex);
	shader->setUniform("gWorldPos", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.worldNormalTex);
	shader->setUniform("gWorldNormal", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.albedoTex);
	shader->setUniform("gAlbedo", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.roughnessMetalnessAoTex);
	shader->setUniform("gRoughnessMetalnessAo", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.emissionTex);
	shader->setUniform("gEmission", 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.depthStencilTex);
	shader->setUniform("gDepth", 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_scene->getSkyBox().getIrradianceMapID());
	shader->setUniform("irradianceMap", 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_scene->getSkyBox().getPrefilterMapID());
	shader->setUniform("prefilterMap", 7);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_scene->getSkyBox().getBRDFLUTTexture());
	shader->setUniform("brdfLUT", 8);

	// bind shadow map
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_shadow.getShadowMap());
	shader->setUniform("shadowMap", 9);
	shader->setUniform("lightSpaceMatrix", m_shadow.getLightSpaceMatrix());

	// bind light data
	m_scene->getLightManager().updateShader(m_scene->getShaderManager().getShader(ShaderName::PBR));

	// draw quad
	glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}
