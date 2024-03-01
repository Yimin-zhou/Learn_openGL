#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

#include "shader.h"
#include "texture.h"

class PBRParameter
{
public:
	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 roughness = glm::vec3(1.0f);
	glm::vec3 metallic = glm::vec3(1.0f);
	glm::vec3 ao = glm::vec3(1.0f);
	float normalStrength = 1.0f;

	// texture
	std::shared_ptr<Texture> albedoMap;
	std::shared_ptr<Texture> roughnessMap;
	std::shared_ptr<Texture> metallicMap;
	std::shared_ptr<Texture> aoMap;
	std::shared_ptr<Texture> normalMap;
};


class Material 
{
public:
	Material();

	void use(const glm::mat4& modelMatrix, const glm::mat4& projectionViewMatrix, const glm::vec3& viewDirection) const;
	void ubind() const;

	void setShader(std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> getShader() const;

	PBRParameter& getPBRParameter() { return pbrParameter; }

	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, int value);
	void setUniform(const std::string& name, const glm::vec2& value);
	void setUniform(const std::string& name, const glm::vec3& value);
	void setUniform(const std::string& name, const glm::vec4& value);
	void setUniform(const std::string& name, const glm::mat3& value);
	void setUniform(const std::string& name, const glm::mat4& value);

public:
	PBRParameter pbrParameter;

private:
	std::shared_ptr<Shader> m_shader;
};

