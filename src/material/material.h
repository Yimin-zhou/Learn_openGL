#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

#include "shader.h"
#include "texture.h"

struct PBRParameter
{
	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);

	// texture
	std::shared_ptr<Texture> albedoMap;
	//std::shared_ptr<Texture> specularMap;
	//std::shared_ptr<Texture> normalMap;
	//std::shared_ptr<Texture> heightMap;
};


class Material 
{
public:
	Material();

	void use(const glm::mat4& modelMatrix, const glm::mat4& viewProjectionMatrix) const;

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

