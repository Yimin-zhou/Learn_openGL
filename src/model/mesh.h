#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "material/material.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

// A mesh entry is a submesh of a mesh
struct MeshEntry 
{
	uint32_t baseVertex;
	uint32_t baseIndex;
	int materialIndex;
	uint32_t numIndices;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices,
		const std::vector<MeshEntry>& meshEntries,
		const std::vector<Material>& materials);
	Mesh() = default;
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	uint32_t getVao() const { return m_vao; }
	uint32_t getVbo() const { return m_vbo; }
	uint32_t getEbo() const { return m_ebo; }
	const std::vector<MeshEntry>& getEntries() const { return m_entries; }
	const std::vector<Material>& getMaterials() const { return m_materials; }

	//use material with index materialIndex
	void useMaterial(int materialIndex, const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix) const;

private:
	uint32_t m_vao;
	uint32_t m_vbo;
	uint32_t m_ebo;
	std::vector<MeshEntry> m_entries; // Submeshes
	std::vector<Material> m_materials;
};
