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
	void setDiffuse(const glm::vec3& value) { m_diffuse = value; }
	void setRoughness(const float value) { m_roughness = value; }
	void setMetallic(const float value) { m_metallic = value; }
	void setAo(const float value) { m_ao = value; }
	void setNormalStrength(float value) { m_normalStrength = value; }

	void setAlbedoMap(std::shared_ptr<Texture> texture) { m_albedoMap = texture; }
	void setRoughnessMap(std::shared_ptr<Texture> texture) { m_roughnessMap = texture; }
	void setMetallicMap(std::shared_ptr<Texture> texture) { m_metallicMap = texture; }
	void setAoMap(std::shared_ptr<Texture> texture) { m_aoMap = texture; }
	void setNormalMap(std::shared_ptr<Texture> texture) { m_normalMap = texture; }

	glm::vec3 getDiffuse() const { return m_diffuse; }
	float getRoughness() const { return m_roughness; }
	float getMetallic() const { return m_metallic; }
	float getAo() const { return m_ao; }
	float getNormalStrength() const { return m_normalStrength; }

	std::shared_ptr<Texture> getAlbedoMap() const { return m_albedoMap; }
	std::shared_ptr<Texture> getRoughnessMap() const { return m_roughnessMap; }
	std::shared_ptr<Texture> getMetallicMap() const { return m_metallicMap; }
	std::shared_ptr<Texture> getAoMap() const { return m_aoMap; }
	std::shared_ptr<Texture> getNormalMap() const { return m_normalMap; }

private:
	glm::vec3 m_diffuse = glm::vec3(1.0f);
	float m_roughness = 1.0f;
	float m_metallic = 1.0f;
	float m_ao = 1.0f;
	float m_normalStrength = 1.0f;

	// texture
	std::shared_ptr<Texture> m_albedoMap;
	std::shared_ptr<Texture> m_roughnessMap;
	std::shared_ptr<Texture> m_metallicMap;
	std::shared_ptr<Texture> m_aoMap;
	std::shared_ptr<Texture> m_normalMap;
};


class Material 
{
public:
	Material();

	void use(const glm::mat4& modelMatrix, const glm::mat4& projectionViewMatrix, const glm::vec3& camPos) const;
	void useEnvironmentMap(uint32_t environmentMap);
	void useIrradianceMap(uint32_t irradianceMap);
	void usePrefilterMap(uint32_t prefilterMap);
	void useBRDFLUTTexture(uint32_t brdfLUTTexture);
	void ubind() const;

	void useGeometryPass(const glm::mat4& modelMatrix, const glm::mat4& projectionViewMatrix, const glm::vec3& cameraPos) const;

	void setShader(std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> getShader() const;

	void setShaderName(ShaderName shaderName) { m_shaderName = shaderName; }
	ShaderName getShaderName() const { return m_shaderName; }

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
	ShaderName m_shaderName;
};

