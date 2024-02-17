#pragma once

#include "light.h"

class DirectionalLight : public Light 
{
public:
	DirectionalLight(const glm::vec3& location, const glm::vec3& color, float intensity);

	void accept(LightVisitor& visitor) override { visitor.visit(*this); }

private:
	glm::vec3 m_direction;
};
