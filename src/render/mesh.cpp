#include "mesh.h"

namespace render
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MeshEntry>& meshEntries, const std::vector<Material>& materials)
		: m_entries(meshEntries), m_materials(materials) 
	{

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture coordinate attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Mesh::~Mesh() 
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo),
		m_entries(std::move(other.m_entries)), m_materials(std::move(other.m_materials)) 
	{
		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept 
	{
		if (this != &other) 
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ebo);

			m_vao = other.m_vao;
			m_vbo = other.m_vbo;
			m_ebo = other.m_ebo;
			m_entries = std::move(other.m_entries);
			m_materials = std::move(other.m_materials);

			other.m_vao = 0;
			other.m_vbo = 0;
			other.m_ebo = 0;
		}
		return *this;
	}

	void Mesh::draw() const
	{
		glBindVertexArray(m_vao);
		for (const auto& entry : m_entries) 
		{
			m_materials[entry.materialIndex].use();

			glDrawElementsBaseVertex(GL_TRIANGLES,
				entry.numIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(uint32_t) * entry.baseIndex),
				entry.baseVertex);
		}
		glBindVertexArray(0);
	}

}