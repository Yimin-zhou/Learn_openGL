#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "shader.h"

namespace render
{
	class Material 
	{
	public:
		Material();
		//explicit Material(const Shader& shader); // Constructor with Shader

		void use() const;

		void setFloat(const std::string& name, float value);
		void setInteger(const std::string& name, int value);
		void setVector2f(const std::string& name, const glm::vec2& value);
		void setVector3f(const std::string& name, const glm::vec3& value);
		void setVector4f(const std::string& name, const glm::vec4& value);
		void setMatrix4f(const std::string& name, const glm::mat4& value);

		// Add methods for setting textures, if necessary

	public:
		float shininess = 32.0f;
		glm::vec3 diffuse = glm::vec3(1.0f);

	private:
		//Shader m_shader;
	};
}