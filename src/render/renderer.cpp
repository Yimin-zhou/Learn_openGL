#include <iostream>

#include "renderer.h"
#include "util/config.h"

Renderer::Renderer()
	: m_camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, 3.0f, 0.05f))
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
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// depth testing
	glEnable(GL_DEPTH_TEST);


	for (auto& model : m_models)
	{
		model.setViewProjectionMatrix(m_camera.getProjectionMatrix(window) * m_camera.getViewMatrix());
		model.draw();
	}
}