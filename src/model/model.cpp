#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>

#include "model.h"
#include "material/texture.h"

Model::Model(const std::filesystem::path& path) :
	m_modelMatrix(1.0f), m_position(0.0f), m_rotation(0.0f), m_scale(1.0f), m_type(ModelType::OPAQUE)
{
	loadMesh(path);
}

Model::Model(const std::filesystem::path& path, ModelType type) :
	m_modelMatrix(1.0f), m_position(0.0f), m_rotation(0.0f), m_scale(1.0f), m_type(type)
{
	loadMesh(path);
}

void Model::setPosition(const glm::vec3& position)
{
	m_position = position;
	updateModelMatrix();
}

void Model::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	updateModelMatrix();
}

void Model::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	updateModelMatrix();
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

void Model::updateModelMatrix()
{
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}

void Model::calculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		Vertex& v0 = vertices[indices[i + 0]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];

		glm::vec3 edge1 = v1.position - v0.position;
		glm::vec3 edge2 = v2.position - v0.position;

		glm::vec2 deltaUV1 = v1.texCoords - v0.texCoords;
		glm::vec2 deltaUV2 = v2.texCoords - v0.texCoords;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		v0.tangent += tangent;
		v1.tangent += tangent;
		v2.tangent += tangent;

		glm::vec3 bitangent = glm::cross(v0.normal, tangent);
		v0.bitangent = bitangent;
		v1.bitangent = bitangent;
		v2.bitangent = bitangent;
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		Vertex& v = vertices[i];
		v.tangent = glm::normalize(v.tangent);
		v.bitangent = glm::normalize(v.bitangent);
	}
}

void Model::loadMesh(const std::filesystem::path& filePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> tinyMaterials;
	std::string warn, err;

	std::filesystem::path parentPath = filePath.parent_path();

	m_name = filePath.stem().string();

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

	for (const auto& mat : tinyMaterials)
	{
		Material material;
		material.pbrParameter.diffuse = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
		material.pbrParameter.albedoMap = std::make_shared<Texture>(parentPath / mat.diffuse_texname);
		material.pbrParameter.roughnessMap = std::make_shared<Texture>(parentPath / mat.roughness_texname);
		material.pbrParameter.metallicMap = std::make_shared<Texture>(parentPath / mat.metallic_texname);
		material.pbrParameter.normalMap = std::make_shared<Texture>(parentPath / mat.normal_texname);
		material.pbrParameter.aoMap = std::make_shared<Texture>(parentPath / mat.ambient_texname);
		// Add more material properties here as needed
		m_materials.push_back(material);
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
		entry.materialIndex = !shape.mesh.material_ids.empty() ? shape.mesh.material_ids[0] : -1; // Use -1 if no material

		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
		{
			int fv = shape.mesh.num_face_vertices[f];

			// Before processing the face, check if we need to start a new entry due to a material change
			int currentMaterialIndex = shape.mesh.material_ids[f];
			if (currentMaterialIndex != entry.materialIndex) 
			{
				// Finalize the current entry if it's not the first iteration
				if (f > 0) 
				{
					entry.numIndices = static_cast<uint32_t>(indices.size()) - entry.baseIndex;
					entries.push_back(entry);
				}
				// Start a new entry
				entry.baseVertex = static_cast<uint32_t>(vertices.size());
				entry.baseIndex = static_cast<uint32_t>(indices.size());
				entry.materialIndex = currentMaterialIndex;
			}

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

				vertices.push_back(Vertex{ position, normal, texCoords });
				indices.push_back(static_cast<uint32_t>(vertices.size() - 1)); // Index of the added vertex
			}
			index_offset += fv;
		}

		// Calculate tangents and bitangents
		calculateTangents(vertices, indices);

		// Add the last entry after finishing all faces
		entry.numIndices = static_cast<uint32_t>(indices.size()) - entry.baseIndex;
		entries.push_back(entry);

		m_meshes.emplace_back(vertices, indices, entries, m_materials);

	}
}


