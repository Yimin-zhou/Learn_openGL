#include <iostream>

#include "renderer.h"
#include "util/config.h"

#include <glm/gtx/string_cast.hpp>

Renderer::Renderer() : 
	m_camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, 3.0f, 0.05f)),
	m_aspectRatio(16.0f / 9.0f), m_finalFrambufferWidth(800), m_finalFrambufferHeight(600),
	m_finalFramebuffer(0), m_finalTexture(0), m_depthBuffer(0), m_shaderManager(ShaderManager()), m_lightManager(LightManager())
{
}

void Renderer::init()
{
	m_models.push_back(Model(modelPath));
	m_shaderManager.buildShader("lambert", "shader/lambert_vert.glsl", "shader/lambert_frag.glsl");
	{
		m_lightManager.addLight(std::make_shared<DirectionalLight>(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.5f), 1.0f));
		m_lightManager.addLight(std::make_shared<PointLight>(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, 1.0f, 0.09f, 0.032f));
	}
}

void Renderer::update(std::shared_ptr<Window> window, float deltaTime)
{
	m_camera.update(window, deltaTime);
}

void Renderer::draw(glm::vec2 renderSize)
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
				else
				{
					Material material = mesh.getMaterials()[entry.materialIndex];
					material.setShader(m_shaderManager.getShader("lambert"));
					material.use(model.getModelMatrix(), m_camera.getProjectionMatrix() * m_camera.getViewMatrix());
					m_lightManager.updateShader(material.getShader());
				}

				glDrawElementsBaseVertex(GL_TRIANGLES,
					entry.numIndices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(uint32_t) * entry.baseIndex),
					entry.baseVertex);
			}
			glBindVertexArray(0);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::resize(glm::vec2 renderSize)
{
	// when window is resized, we need to resize the framebuffer
	// init final framebuffer texture
	glGenTextures(1, &m_finalTexture);
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderSize.x, renderSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification filter
	glBindTexture(GL_TEXTURE_2D, 0);

	// init final framebuffer
	glGenFramebuffers(1, &m_finalFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_finalFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_finalTexture, 0);

	// init depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderSize.x, renderSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::setCameraAspectRatio(float aspectRatio)
{
	m_camera.setAspectRatio(aspectRatio);
}
