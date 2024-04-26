#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "mesh.h"
#include "material/material.h"

enum class ModelType
{
	OPAQUE,
	SIZE
};

class Model
{
public:
	Model(const std::filesystem::path& path);
	Model(const std::filesystem::path& path, ModelType type);

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	const glm::mat4& getModelMatrix() { return m_modelMatrix; }
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	const std::string& getName() { return m_name; }
	const ModelType& getType() { return m_type; }

	// returns the meshes reference
	int getMeshCount() { return m_meshes.size(); }
	const std::vector<Mesh>& getMeshes() { return m_meshes; }
	const Mesh& getMesh(int index) { return m_meshes[index]; }

	std::vector<Material>& getMaterials() { return m_materials; }
	void setMaterial(int index, const Material& material) { m_materials[index] = material; }
	void setMaterials(const std::vector<Material>& materials) { m_materials = materials; }

	void rotateModelHorizontal(float speed);

private:
	ModelType m_type;
	std::string m_name;

	std::vector<Mesh> m_meshes;
	std::vector<Material> m_materials;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_modelMatrix;

	void loadMesh(const std::filesystem::path& filePath);
	void updateModelMatrix();

	void calculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);


};
