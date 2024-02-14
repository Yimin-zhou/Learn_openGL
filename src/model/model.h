#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "mesh.h"
#include "material/material.h"

class Model
{
public:
	Model(const std::filesystem::path& path);


	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	const glm::mat4& getModelMatrix() { return m_modelMatrix; }
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	// returns the meshes reference
	int getMeshCount() { return m_meshes.size(); }
	const std::vector<Mesh>& getMeshes() { return m_meshes; }
	const std::vector<Material>& getMaterials() { return m_materials; }

private:
	std::vector<Mesh> m_meshes;
	std::vector<Material> m_materials;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_modelMatrix;

private:
	void m_loadMesh(const std::filesystem::path& filePath);
	void m_updateModelMatrix();
};
