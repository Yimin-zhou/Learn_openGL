#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

#include "shader.h"

struct PBRParameter
{
	glm::vec3 albedo = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	/*float metallic = 0.0f;
	float roughness = 0.0f;
	float ao = 1.0f;*/
};

class Material 
{
public:
	Material();

	void use() const;

	// get the shader
	std::shared_ptr<Shader> getShader() const;

	// Add methods for setting textures, if necessary

public:
	// shader path
	std::filesystem::path vertexShaderPath = "shader/lambert_vert.glsl";
	std::filesystem::path fragmentShaderPath = "shader/lambert_frag.glsl";

	PBRParameter pbrParameter;

private:
	void m_generateShaders();

private:
	std::shared_ptr<Shader> m_shader;
};

