#include <iostream>

#include "renderer.h"
#include "util/config.h"

namespace core
{
	Renderer::Renderer(int height, int width)
		: m_height(height), 
		m_width(width)
	{

	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}

	void Renderer::init()
	{
		m_generateBuffers();

		std::filesystem::path vtPath = "shader/lambert_vert.glsl";
		std::filesystem::path fgPath = "shader/lambert_frag.glsl";
		m_generateShaders(vtPath, fgPath);
	}

	void Renderer::draw()
	{
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_useShader();
		glBindVertexArray(m_vao); // bind VAO, which automatically binds EBO
		glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		//// test triangle
		//glBegin(GL_TRIANGLES);
		//glVertex3f(-0.5f, -0.5f, 0.0f); // Left
		//glVertex3f(0.5f, -0.5f, 0.0f); // Right
		//glVertex3f(0.0f, 0.5f, 0.0f); // Top
		//glEnd();
	}

	void Renderer::clear()
	{

	}

	void Renderer::m_generateBuffers()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_triangle), m_triangle, GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture coordinate attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// unbind, so other calls won't accidentally modify our VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void Renderer::m_generateShaders(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
	{
		render::ShaderBuilder lambertBuilder;
		lambertBuilder.addStage(GL_VERTEX_SHADER, vertexShaderPath);
		lambertBuilder.addStage(GL_FRAGMENT_SHADER, fragmentShaderPath);
		m_lambertShader = lambertBuilder.build();
	}

	void Renderer::m_useShader()
	{
		m_lambertShader.bind();
		m_lambertShader.setUniform("color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}

}