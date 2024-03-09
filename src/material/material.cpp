#include <iostream>

#include "material.h"

#include "GL/glew.h"

Material::Material()
	: m_shaderName(ShaderName::PBR)
{
}


void Material::use(const glm::mat4& modelMatrix, const glm::mat4& projectionViewMatrix, const glm::vec3& cameraPos) const
{
	// Set matrices
	m_shader->setUniform("modelMatrix", modelMatrix);
	m_shader->setUniform("projectionViewMatrix", projectionViewMatrix);

	// Set material properties
	m_shader->setUniform("baseColor", glm::vec4(pbrParameter.getDiffuse(), 1.0f));
	m_shader->setUniform("roughnessStrength", pbrParameter.getRoughness());
	m_shader->setUniform("metallicStrength", pbrParameter.getMetallic());

	m_shader->setUniform("cameraPos", cameraPos);

	// Set textures
	pbrParameter.getAlbedoMap()->bind(0);
	m_shader->setUniform("albedoMap", 0);
	pbrParameter.getNormalMap()->bind(1);
	m_shader->setUniform("normalMap", 1);
	pbrParameter.getMetallicMap()->bind(2);
	m_shader->setUniform("metallicMap", 2);
	pbrParameter.getRoughnessMap()->bind(3);
	m_shader->setUniform("roughnessMap", 3);
	pbrParameter.getAoMap()->bind(4);
	m_shader->setUniform("aoMap", 4);

}

void Material::useEnvironmentMap(uint32_t environmentMap)
{
	// bind environment map
	m_shader->setUniform("environmentMap", 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMap);
}

void Material::useIrradianceMap(uint32_t irradianceMap)
{
	m_shader->setUniform("irradianceMap", 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
}

void Material::usePrefilterMap(uint32_t prefilterMap)
{
	m_shader->setUniform("prefilterMap", 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
}

void Material::useBRDFLUTTexture(uint32_t brdfLUTTexture)
{
	m_shader->setUniform("brdfLUT", 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

void Material::ubind() const
{
	pbrParameter.getAlbedoMap()->unbind();
	pbrParameter.getNormalMap()->unbind();
	pbrParameter.getMetallicMap()->unbind();
	pbrParameter.getRoughnessMap()->unbind();
}

void Material::useGeometryPass(const glm::mat4& modelMatrix, const glm::mat4& projectionViewMatrix, const glm::vec3& cameraPos) const
{
	// Set matrices
	m_shader->setUniform("modelMatrix", modelMatrix);
	m_shader->setUniform("projectionViewMatrix", projectionViewMatrix);

	// Set material properties
	m_shader->setUniform("baseColor", glm::vec4(pbrParameter.getDiffuse(), 1.0f));
	m_shader->setUniform("roughnessStrength", pbrParameter.getRoughness());
	m_shader->setUniform("metallicStrength", pbrParameter.getMetallic());

	m_shader->setUniform("cameraPos", cameraPos);

	// Set textures
	pbrParameter.getAlbedoMap()->bind(0);
	m_shader->setUniform("albedoMap", 0);
	pbrParameter.getNormalMap()->bind(1);
	m_shader->setUniform("normalMap", 1);
	pbrParameter.getMetallicMap()->bind(2);
	m_shader->setUniform("metallicMap", 2);
	pbrParameter.getRoughnessMap()->bind(3);
	m_shader->setUniform("roughnessMap", 3);
	pbrParameter.getAoMap()->bind(4);
	m_shader->setUniform("aoMap", 4);
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
