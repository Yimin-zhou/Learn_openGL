#include <iostream>

#include "renderer.h"
#include "util/config.h"

Renderer::Renderer()
{
}

void Renderer::init()
{
	m_models.push_back(Model(modelPath));
}

void Renderer::draw()
{
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& model : m_models)
	{
		model.draw();
	}
}