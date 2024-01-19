#include "model.h"

namespace render
{
	Model::Model()
	{
		m_loadMesh("assets/models/monkey.obj");
	}

	Model::Model(std::filesystem::path path)
	{
		m_loadMesh(path);
	}

	void Model::draw()
	{
		for (int i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].draw();
		}
	}


	void Model::m_loadMesh(const std::filesystem::path& filePath)
	{

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> tinyMaterials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &tinyMaterials, &warn, &err, filePath.string().c_str()))
		{
			throw std::runtime_error(err);
		}

		for (const auto& mat : tinyMaterials)
		{
			render::Material material;
			material.diffuse = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
			// Add more material properties here as needed
			m_materials.push_back(material);
		}

		for (const tinyobj::shape_t& shape : shapes)
		{
			std::vector<render::Vertex> vertices;
			std::vector<uint32_t> indices;
			std::vector<render::MeshEntry> entries;

			size_t index_offset = 0;
			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
			{
				int fv = shape.mesh.num_face_vertices[f];

				render::MeshEntry entry;
				entry.baseVertex = static_cast<uint32_t>(vertices.size());
				entry.baseIndex = static_cast<uint32_t>(index_offset);
				entry.numIndices = fv;
				entry.materialIndex = shape.mesh.material_ids[f];

				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

					glm::vec3 position(attrib.vertices[3 * idx.vertex_index + 0],
						attrib.vertices[3 * idx.vertex_index + 1],
						attrib.vertices[3 * idx.vertex_index + 2]);
					glm::vec3 normal(attrib.normals[3 * idx.normal_index + 0],
						attrib.normals[3 * idx.normal_index + 1],
						attrib.normals[3 * idx.normal_index + 2]);
					glm::vec2 texCoords(attrib.texcoords[2 * idx.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]);

					vertices.push_back(render::Vertex{position, normal, texCoords});
					indices.push_back(static_cast<uint32_t>(index_offset + v));
				}
				index_offset += fv;

				entries.push_back(entry);
			}

			m_meshes.emplace_back(vertices, indices, entries, m_materials);
		}
	}
}

