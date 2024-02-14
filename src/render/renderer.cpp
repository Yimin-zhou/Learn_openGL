#include <iostream>

#include <glm/gtx/string_cast.hpp>

#include "renderer.h"
#include "util/config.h"

Renderer::Renderer() : 
	m_camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, 3.0f, 0.05f)),
	m_aspectRatio(16.0f / 9.0f), m_finalFrambufferWidth(800), m_finalFrambufferHeight(600)
{

}

void Renderer::init()
{
	m_models.push_back(Model(modelPath));
}

void Renderer::update(std::shared_ptr<Window> window, float deltaTime)
{
	m_camera.update(window, deltaTime);
}

void Renderer::draw(std::shared_ptr<Window> window)
{
	// render pass
	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_finalFramebuffer);
	glClearColor(0.21f, 0.22f, 0.247f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->getRenderSize().x, window->getRenderSize().y);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	for (auto& model : m_models)
	{
		for (const auto& mesh : model.getMeshes())
		{
			glBindVertexArray(mesh.getVao());
			for (const auto& entry : mesh.getEntries())
			{
				int materialIndex = entry.materialIndex;
				if (materialIndex == -1)
				{
					materialIndex = 0;
				}
				mesh.useMaterial(materialIndex,model.getModelMatrix(), m_camera.getProjectionMatrix(window));

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

void Renderer::resize(std::shared_ptr<Window> window)
{
	// when window is resized, we need to resize the framebuffer
	// init final framebuffer texture
	glGenTextures(1, &m_finalTexture);
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->getRenderSize().x, window->getRenderSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->getRenderSize().x, window->getRenderSize().y);
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
