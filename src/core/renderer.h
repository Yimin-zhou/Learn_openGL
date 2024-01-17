#pragma once
#include "util/shader.h"

#include <string>

namespace core
{
	class Renderer
	{
	public:
		Renderer(int height, int width);
		~Renderer();

		void init();
		void draw();
		void clear();


	private:
		void m_generateBuffers();
		void m_generateShaders(const std::filesystem::path& vertexShaderSource, const std::filesystem::path& fragmentShaderSource);

	private:
		util::Shader m_lambertShader;

	private:
		int m_height;
		int m_width;

		uint32_t m_vao;
		uint32_t m_vbo;
		uint32_t m_ebo;

		uint32_t m_vertexShader;
		uint32_t m_fragmentShader;
		uint32_t m_shaderProgram;

		// A test triangle
		uint32_t m_numIndices = 3;
		uint32_t m_indices[3] =
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
