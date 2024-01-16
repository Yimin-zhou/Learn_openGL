#include <iostream>

#include "renderer.h"

namespace core
{
	Renderer::Renderer(uint32_t height, uint32_t width)
		: m_height(height), m_width(width)
	{

	}

	void Renderer::init()
	{
		std::string vtPath = "../shader/lambert_vert.glsl";
		std::string fgPath = "../shader/lambert_frag.glsl";
		m_useShader(fgPath, vtPath);
		m_generateBuffers();

	}

	void Renderer::draw()
	{
		glViewport(0, 0, m_height, m_width);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glBindVertexArray(m_vao); // bind VAO, which automatically binds EBO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Renderer::clear()
	{

	}

	void Renderer::m_generateBuffers()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_triangle), m_triangle, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// this is "layout (location = 0)" in vertex shader

		// normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(12));

		// texture attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(24));

		// unbind, so other calls won't accidentally modify our VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void Renderer::m_useShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		util::Shader shader(vertexShaderSource, fragmentShaderSource);
		shader.bind();

	}
}