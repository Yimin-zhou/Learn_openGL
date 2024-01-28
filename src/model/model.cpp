#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>

#include "model.h"
#include "material/texture.h"

Model::Model(const std::filesystem::path& path) :
	m_modelMatrix(1.0f), m_viewProjectionMatrix(1.0f), m_position(0.0f), m_rotation(0.0f), m_scale(1.0f)
{
	m_loadMesh(path);
}

void Model::draw()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].draw(m_modelMatrix, m_viewProjectionMatrix);
	}
}

void Model::setPosition(const glm::vec3& position)
{
	m_position = position;
	m_updateModelMatrix();
}

void Model::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	m_updateModelMatrix();
}

void Model::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	m_updateModelMatrix();
}

void Model::setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix)
{
	m_viewProjectionMatrix = viewProjectionMatrix;
}

glm::vec3 Model::getPosition()
{
	return m_position;
}

glm::vec3 Model::getRotation()
{
	return m_rotation;
}

glm::vec3 Model::getScale()
{
	return m_scale;
}

void Model::m_updateModelMatrix()
{
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}

void Model::m_loadMesh(const std::filesystem::path& filePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> tinyMaterials;
	std::string warn, err;

	std::filesystem::path parentPath = filePath.parent_path();

	if (!tinyobj::LoadObj(&attrib, &shapes, &tinyMaterials, &warn, &err, filePath.string().c_str(), parentPath.string().c_str()))
	{
		throw std::runtime_error(err);
	}
	if (!warn.empty()) {

		std::cout << "Warning: " << warn << std::endl;
	}
	if (!err.empty()) 
	{
		std::cerr << "Error: " << err << std::endl;
	}
	// TODO: create default materials
	if (tinyMaterials.empty())
	{
		Material material;
		material.pbrParameter.diffuse = glm::vec3(1.0f);
		// Add more material properties here as needed
		m_materials.push_back(material);
	}
	else
	{
		for (const auto& mat : tinyMaterials)
		{
			Material material;
			material.pbrParameter.diffuse = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
			material.pbrParameter.albedoMap = std::make_shared<Texture>(parentPath / mat.diffuse_texname);
			// Add more material properties here as needed
			m_materials.push_back(material);
		}
	}

	for (const tinyobj::shape_t& shape : shapes) 
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<MeshEntry> entries;

		size_t index_offset = 0;
		MeshEntry entry;
		entry.baseVertex = 0;
		entry.baseIndex = 0;
		entry.materialIndex = !shape.mesh.material_ids.empty() ? shape.mesh.material_ids[0] : 0;

		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
		{
			int fv = shape.mesh.num_face_vertices[f];
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

				vertices.push_back(Vertex{position, normal, texCoords});
				indices.push_back(static_cast<uint32_t>(index_offset + v));
			}
			index_offset += fv;

			// Check if the next face uses a different material
			if (f + 1 < shape.mesh.material_ids.size() && shape.mesh.material_ids[f + 1] != entry.materialIndex) 
			{
				entry.numIndices = static_cast<uint32_t>(indices.size()) - entry.baseIndex;
				entries.push_back(entry);

				// Start a new entry
				entry.baseVertex = static_cast<uint32_t>(vertices.size());
				entry.baseIndex = static_cast<uint32_t>(indices.size());
				entry.materialIndex = shape.mesh.material_ids[f + 1];
			}
		}

		// Add the last entry
		entry.numIndices = static_cast<uint32_t>(indices.size()) - entry.baseIndex;
		entries.push_back(entry);

		m_meshes.emplace_back(vertices, indices, entries, m_materials);
	}
}


