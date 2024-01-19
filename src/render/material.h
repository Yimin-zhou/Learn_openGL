#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

#include "shader.h"

namespace render
{
	class Material 
	{
	public:
		Material();
		//explicit Material(const Shader& shader); // Constructor with Shad

		void use() const;

		// get the shader
		std::shared_ptr<Shader> getShader() const;

		// Add methods for setting textures, if necessary

	public:
		float shininess = 32.0f;
		glm::vec3 diffuse = glm::vec3(1.0f);

	private:
		void m_generateShaders(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

	private:
		std::shared_ptr<Shader> m_shader;
	};
}