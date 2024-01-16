#pragma once

#include <GL/glew.h>

#include <string>

namespace util
{
	class Shader
	{
	public:
		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		uint32_t get() { return m_shaderProgram; }

		void bind();
		void unbind();

		void setUniform1i(const std::string& name, int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	private:
		static uint32_t m_compileShader(GLenum type, const std::string& source);

	private:
		uint32_t m_shaderProgram;
	};
}