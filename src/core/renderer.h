#pragma once
#include "../util/shader.h"

#include <string>

namespace core
{
	class Renderer
	{
	public:
		Renderer();

		void init();
		void draw();
		void clear();


	private:
		void m_generateBuffers();
		void m_useShader(const std::string& vertexShaderSource ,const std::string& fragmentShaderSource);

	private:
		uint32_t m_vao;
		uint32_t m_vbo;
		uint32_t m_ebo;

		uint32_t m_vertexShader;
		uint32_t m_fragmentShader;
		uint32_t m_shaderProgram;

		// A test triangle
		float m_indices[6] = 
		{
			0, 1, 2
		};

		float m_triangle[24] =
		{
			// positions			// normals			// texture coords
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// bottom left
			0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	// bottom right
			0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// top right
		};
	};
}
