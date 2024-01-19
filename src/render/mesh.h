#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "material.h"

namespace render
{
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

		void draw() const;

	private:
		uint32_t m_vao;
		uint32_t m_vbo;
		uint32_t m_ebo;
		std::vector<MeshEntry> m_entries;
		std::vector<Material> m_materials;

		// A test triangle
		//uint32_t m_numIndices = 3;
		//uint32_t m_indices[3] =
		//{
		//	0, 1, 2
		//};

		//float m_triangle[24] =
		//{
		//	// positions			// normals			// texture coords
		//	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// bottom left
		//	0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	// bottom right
		//	0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// top right
		//};
	};
}