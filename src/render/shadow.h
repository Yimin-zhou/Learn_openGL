#pragma once

#include <GL/glew.h>
#include <stdint.h>
#include <vector>

#include "model/model.h"

class Shadow
{
public:
	Shadow(uint32_t width, uint32_t height);
	Shadow() = default;

	void renderShadowMap(std::vector<Model>& models, std::shared_ptr<Shader> shader, const glm::vec3& lightDir);

	uint32_t getShadowMap() const { return m_shadowMap; }

	const glm::mat4& getLightSpaceMatrix() { return m_lightSpaceMatrix; }

private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_depthMapFBO;
	uint32_t m_shadowMap;

	glm::mat4 m_lightSpaceMatrix;

	void createShadowMap();

};

