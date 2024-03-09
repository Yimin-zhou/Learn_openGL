#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>
#include <filesystem>
#include <vector>
#include <memory>
#include <unordered_map>

struct ShaderLoadingException : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};

enum class ShaderName
{
	LAMBERT,
	PBR,

	SKYBOX,

	GBUFFER,
	CONVOLUTION,
	PREFILTER,
	PRE_BRDF,

	SIZE
};

class Shader
{
public:
	Shader(uint32_t program);
	Shader();
	~Shader();
	Shader(Shader&&);
	Shader& operator=(Shader&&);

	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, int value);
	void setUniform(const std::string& name, const glm::vec2& value);
	void setUniform(const std::string& name, const glm::vec3& value);
	void setUniform(const std::string& name, const glm::vec4& value);
	void setUniform(const std::string& name, const glm::mat3& value);
	void setUniform(const std::string& name, const glm::mat4& value);
	void bind() const;
	void unbind() const;

private:
	friend class ShaderBuilder;
	uint32_t m_program;
	mutable std::unordered_map<std::string, int32_t> m_uniformLocations;

	int32_t getUniformLocation(const std::string& name) const;
};

class ShaderBuilder
{
public:
	ShaderBuilder() = default;
	ShaderBuilder(const ShaderBuilder&) = delete;
	~ShaderBuilder();

	ShaderBuilder& addStage(uint32_t shaderStage, const std::filesystem::path& shaderFile);
	std::shared_ptr<Shader> build();

private:
	std::vector<uint32_t> m_shaders;
	void freeShaders();
};

class ShaderManager
{
public:
	ShaderManager() = default;

	void buildShader(ShaderName name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragPath);
	void buildComputeShader(ShaderName name, const std::filesystem::path& computePath);
	std::shared_ptr<Shader> getShader(ShaderName name) const;

private:
	std::unordered_map<ShaderName, std::shared_ptr<Shader>> m_shaders;
};

