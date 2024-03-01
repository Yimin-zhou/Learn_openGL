#include <iostream>

#include "material.h"

Material::Material()
{
}


void Material::use(const glm::mat4& modelMatrix, const glm::mat4& projectionViewMatrix, const glm::vec3& cameraPos) const
{
	// Set matrices
	m_shader->setUniform("modelMatrix", modelMatrix);
	m_shader->setUniform("projectionViewMatrix", projectionViewMatrix);

	// Set material properties
	m_shader->setUniform("baseColor", glm::vec4(pbrParameter.diffuse, 1.0f));
	m_shader->setUniform("cameraPos", cameraPos);

	// Set textures
	pbrParameter.albedoMap->bind(0);
	m_shader->setUniform("albedoMap", 0);
	pbrParameter.normalMap->bind(1);
	m_shader->setUniform("normalMap", 1);
	pbrParameter.metallicMap->bind(2);
	m_shader->setUniform("metallicMap", 2);
	pbrParameter.roughnessMap->bind(3);
	m_shader->setUniform("roughnessMap", 3);
	pbrParameter.aoMap->bind(4);

}

void Material::ubind() const
{
	pbrParameter.albedoMap->unbind();
	pbrParameter.normalMap->unbind();
	pbrParameter.metallicMap->unbind();
	pbrParameter.roughnessMap->unbind();
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
