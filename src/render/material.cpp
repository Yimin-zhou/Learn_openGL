#include "material.h"

namespace render
{

	Material::Material()
	{

	}

	//Material::Material(const Shader& shader) : m_shader(std::move(shader)) {}

	void Material::use() const 
	{
		//m_shader.bind(); // Assuming Shader class has a use() method
		// Set global material properties
		//m_shader.setUniform("material.shininess", shininess);
		//m_shader.setUniform("material.diffuse", diffuse);
		// Set other material properties
	}

	void Material::setFloat(const std::string& name, float value) 
	{
		//m_shader.setUniform(name, value);
	}

	void Material::setInteger(const std::string& name, int value) 
	{
		//m_shader.setUniform(name, value);
	}

	void Material::setVector2f(const std::string& name, const glm::vec2& value) 
	{
		//m_shader.setUniform(name, value);
	}

	void Material::setVector3f(const std::string& name, const glm::vec3& value) 
	{
		//m_shader.setUniform(name, value);
	}

	void Material::setVector4f(const std::string& name, const glm::vec4& value) 
	{
		//m_shader.setUniform(name, value);
	}

	void Material::setMatrix4f(const std::string& name, const glm::mat4& value) 
	{
		//m_shader.setUniform(name, value);
	}

}