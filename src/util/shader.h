#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>
#include <filesystem>
#include <vector>

namespace util
{
	struct ShaderLoadingException : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	class Shader
	{
	public:
		Shader();
		~Shader();
		Shader(Shader&&);
		Shader& operator=(Shader&&);

	public:
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, const glm::vec3& value);
		void SetUniform(const std::string& name, const glm::vec4& value);
		void SetUniform(const std::string& name, const glm::mat3& value);
		void SetUniform(const std::string& name, const glm::mat4& value);
		void bind() const;

	private:
		friend class ShaderBuilder;
		Shader(uint32_t program);

	private:
		uint32_t m_program;
	};

	class ShaderBuilder
	{
	public:
		ShaderBuilder() = default;
		ShaderBuilder(const ShaderBuilder&) = delete;
		~ShaderBuilder();

		ShaderBuilder& addStage(uint32_t shaderStage, std::filesystem::path shaderFile);
		Shader build();

	private:
		void freeShaders();

	private:
		std::vector<uint32_t> m_shaders;
	};
}
