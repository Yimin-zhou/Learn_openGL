#pragma once

#include <filesystem>

#include <glm/glm.hpp>


#include "mesh.h"
#include "material.h"

namespace render
{
	class Model
	{
	public:
		Model(std::filesystem::path path);

		void draw();

		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setScale(glm::vec3 scale);

		glm::vec3 getPosition();
		glm::vec3 getRotation();
		glm::vec3 getScale();

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
}