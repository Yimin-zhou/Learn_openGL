#include "material.h"

Material::Material()
{
	m_generateShaders();
}

void Material::use() const 
{
	m_shader->bind(); // Assuming Shader class has a use() method
	// Set global material properties
	m_shader->setUniform("diffuse", glm::vec4(pbrParameter.diffuse, 1.0f));
	//m_shader.setUniform("material.shininess", shininess);
	// Set other material properties
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
