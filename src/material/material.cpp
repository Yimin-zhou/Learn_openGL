#include "material.h"

Material::Material()
{
	m_generateShaders();
}

void Material::use(const glm::mat4& modelMatrix, const glm::mat4& viewProjectionMatrix) const
{
	m_shader->bind();

	// Set matrices
	m_shader->setUniform("modelMatrix", modelMatrix);
	m_shader->setUniform("viewProjectionMatrix", viewProjectionMatrix);

	// Set material properties
	m_shader->setUniform("diffuse", glm::vec4(pbrParameter.diffuse, 1.0f));

	// Set textures
	pbrParameter.albedoMap->bind(0);
	m_shader->setUniform("albedoMap", 0);

}

std::shared_ptr<Shader> Material::getShader() const
{
	return m_shader;
}

void Material::m_generateShaders()
{
	ShaderBuilder lambertBuilder;
	lambertBuilder.addStage(GL_VERTEX_SHADER, vertexShaderPath);
	lambertBuilder.addStage(GL_FRAGMENT_SHADER, fragmentShaderPath);
	m_shader = lambertBuilder.build();
}
