#include "material.h"

namespace render
{

	Material::Material()
	{
		std::filesystem::path vtPath = "shader/lambert_vert.glsl";
		std::filesystem::path fgPath = "shader/lambert_frag.glsl";
		m_generateShaders(vtPath, fgPath);
	}

	//Material::Material(const Shader& shader) : m_shader(std::move(shader)) {}

	void Material::use() const 
	{
		m_shader->bind(); // Assuming Shader class has a use() method
		// Set global material properties
		//m_shader.setUniform("material.shininess", shininess);
		//m_shader.setUniform("material.diffuse", diffuse);
		// Set other material properties
	}


	std::shared_ptr<render::Shader> Material::getShader() const
	{
		return m_shader;
	}

	void Material::m_generateShaders(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
	{
		render::ShaderBuilder lambertBuilder;
		lambertBuilder.addStage(GL_VERTEX_SHADER, vertexShaderPath);
		lambertBuilder.addStage(GL_FRAGMENT_SHADER, fragmentShaderPath);
		m_shader = lambertBuilder.build();
	}

}