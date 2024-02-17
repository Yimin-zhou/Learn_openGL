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

class Shader
{
public:
	Shader(uint32_t program);
	Shader();
	~Shader();
	Shader(Shader&&);
	Shader& operator=(Shader&&);

public:
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

private:
	uint32_t m_program;
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
	void freeShaders();

private:
	std::vector<uint32_t> m_shaders;
};

class ShaderManager
{
public:
	ShaderManager() = default;

	void buildShader(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragPath);
	std::shared_ptr<Shader> getShader(const std::string& name) const;

private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

};

