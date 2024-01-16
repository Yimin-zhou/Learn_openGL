#include <iostream>

#include "shader.h"

namespace util
{
	Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		// create shader program
		uint32_t shaderProgram = glCreateProgram();
		uint32_t vertexShader = m_compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		uint32_t fragmentShader = m_compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glValidateProgram(shaderProgram);

		// delete shaders, since they are already linked to the program
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_shaderProgram = shaderProgram;
	}

	void Shader::bind()
	{
		glUseProgram(m_shaderProgram);
	}

	void Shader::unbind()
	{

	}

	void Shader::setUniform1i(const std::string& name, int value)
	{

	}

	void Shader::setUniform1f(const std::string& name, float value)
	{

	}

	void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{

	}

	uint32_t Shader::m_compileShader(GLenum type, const std::string& source)
	{
		uint32_t shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		// error handling
		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, message);

			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;

			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}
}
