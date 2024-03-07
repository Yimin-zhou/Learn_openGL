#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <format>

#include <GL/glew.h>

#include "shader.h"

static constexpr uint32_t invalid = 0xFFFFFFFF;

static bool checkShaderErrors(uint32_t shader);
static bool checkProgramErrors(uint32_t program);
static std::string readFile(const std::filesystem::path& filePath);

Shader::Shader(uint32_t program)
	: m_program(program)
{
}

Shader::Shader()
	: m_program(invalid)
{
}

Shader::Shader(Shader&& other)
{
	m_program = other.m_program;
	other.m_program = invalid;
}

Shader::~Shader()
{
	if (m_program != invalid)
		glDeleteProgram(m_program);
}


Shader& Shader::operator=(Shader&& other)
{
	if (m_program != invalid)
		glDeleteProgram(m_program);

	m_program = other.m_program;
	other.m_program = invalid;
	return *this;
}

void Shader::setUniform(const std::string& name, float value)
{
	int32_t location = getUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, int value)
{
	int32_t location = getUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, const glm::vec2& value)
{
	int32_t location = getUniformLocation(name);
	glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::vec3& value)
{
	int32_t location = getUniformLocation(name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::vec4& value)
{
	int32_t location = getUniformLocation(name);
	glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value)
{
	int32_t location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value)
{
	int32_t location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::bind() const
{
	assert(m_program != invalid);
	glUseProgram(m_program);
}

void Shader::unbind() const
{
	assert(m_program != invalid);
	glUseProgram(0);
}

int32_t Shader::getUniformLocation(const std::string& name) const
{
	// check if the uniform location is already cached
	if (m_uniformLocations.find(name) != m_uniformLocations.end())
	{
		return m_uniformLocations[name];
	}

	// if not, query the location and cache it
	int32_t location = glGetUniformLocation(m_program, name.c_str());
	if (location == -1) std::cerr << "Uniform " << name << " not found" << std::endl;
	m_uniformLocations[name] = location;
	return location;
}

// ShaderBuilder implementation
ShaderBuilder::~ShaderBuilder()
{
	freeShaders();
}

ShaderBuilder& ShaderBuilder::addStage(uint32_t shaderType, const std::filesystem::path& shaderFile)
{
	if (!std::filesystem::exists(shaderFile)) 
	{
		throw ShaderLoadingException(std::format("File {} does not exist", shaderFile.string().c_str()));
	}

	const std::string shaderSource = readFile(shaderFile);
	const uint32_t shader = glCreateShader(shaderType);
	const char* shaderSourcePtr = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
	glCompileShader(shader);
	if (!checkShaderErrors(shader)) 
	{
		glDeleteShader(shader);
		throw ShaderLoadingException(std::format("Failed to compile shader {}", shaderFile.string().c_str()));
	}

	m_shaders.push_back(shader);
	return *this;
}

std::shared_ptr<Shader> ShaderBuilder::build()
{
	// Combine all shaders into a single shader program.
	uint32_t program = glCreateProgram();
	for (uint32_t shader : m_shaders)
	{
		glAttachShader(program, shader);
	}
	glLinkProgram(program);
	freeShaders();

	if (!checkProgramErrors(program)) 
	{
		throw ShaderLoadingException("Shader program failed to link");
	}

	return std::make_shared<Shader>(program);
}

void ShaderBuilder::freeShaders()
{
	for (uint32_t shader : m_shaders)
	{
		glDeleteShader(shader);
	}
}

static std::string readFile(const std::filesystem::path& filePath)
{
	std::ifstream file(filePath, std::ios::binary);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

static bool checkShaderErrors(uint32_t shader)
{
	// Check if the shader compiled successfully.
	int32_t compileSuccessful;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccessful);

	// If it didn't, then read and print the compile log.
	if (!compileSuccessful) 
	{
		int32_t logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::string logBuffer;
		logBuffer.resize(static_cast<size_t>(logLength));
		glGetShaderInfoLog(shader, logLength, nullptr, logBuffer.data());

		std::cerr << logBuffer << std::endl;
		return false;
	}
	else 
	{
		return true;
	}
}

static bool checkProgramErrors(uint32_t program)
{
	// Check if the program linked successfully
	int32_t linkSuccessful;
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccessful);

	// If it didn't, then read and print the link log
	if (!linkSuccessful) 
	{
		int32_t logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::string logBuffer;
		logBuffer.resize(static_cast<size_t>(logLength));
		glGetProgramInfoLog(program, logLength, nullptr, logBuffer.data());

		std::cerr << logBuffer << std::endl;
		return false;
	}
	else 
	{
		return true;
	}
}

void ShaderManager::buildShader(ShaderName name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragPath)
{
	ShaderBuilder defaultShaderBuilder;
	defaultShaderBuilder.addStage(GL_VERTEX_SHADER, vertexPath);
	defaultShaderBuilder.addStage(GL_FRAGMENT_SHADER, fragPath);
	m_shaders[name] = defaultShaderBuilder.build();
}

void ShaderManager::buildComputeShader(ShaderName name, const std::filesystem::path& computePath)
{
	ShaderBuilder computeShaderBuilder;
	computeShaderBuilder.addStage(GL_COMPUTE_SHADER, computePath);
	m_shaders[name] = computeShaderBuilder.build();
}

std::shared_ptr<Shader> ShaderManager::getShader(ShaderName name) const
{
	if (m_shaders.find(name) == m_shaders.end())
	{
		return m_shaders.at(ShaderName::PBR);
	}
	return m_shaders.at(name);
}
