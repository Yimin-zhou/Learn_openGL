#include <iostream>

#include "material.h"

Material::Material()
{
}


void Material::use(const glm::mat4& modelMatrix, const glm::mat4& viewProjectionMatrix) const
{
	// Set matrices
	m_shader->setUniform("modelMatrix", modelMatrix);
	m_shader->setUniform("viewProjectionMatrix", viewProjectionMatrix);

	// Set material properties
	m_shader->setUniform("diffuse", glm::vec4(pbrParameter.diffuse, 1.0f));

	// Set textures
	pbrParameter.albedoMap->bind(0);
	m_shader->setUniform("albedoMap", 0);

}

void Material::setShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;
	m_shader->bind();
}

std::shared_ptr<Shader> Material::getShader() const
{
	return m_shader;
}

void Material::setUniform(const std::string& name, float value)
{
	m_shader->setUniform(name, value);
}

void Material::setUniform(const std::string& name, int value)
{
	m_shader->setUniform(name, value);
}

void Material::setUniform(const std::string& name, const glm::vec2& value)
{
	m_shader->setUniform(name, value);
}

void Material::setUniform(const std::string& name, const glm::vec3& value)
{
	m_shader->setUniform(name, value);
}

void Material::setUniform(const std::string& name, const glm::vec4& value)
{
	m_shader->setUniform(name, value);
}

void Material::setUniform(const std::string& name, const glm::mat3& value)
{
	m_shader->setUniform(name, value);
}

void Material::setUniform(const std::string& name, const glm::mat4& value)
{
	m_shader->setUniform(name, value);
}
