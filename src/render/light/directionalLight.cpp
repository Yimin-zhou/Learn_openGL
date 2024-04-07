#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity) :
	Light(LightType::DIRECTIONAL, direction, color, intensity) // Directional light has no position
{
	this->setDirection(direction);
}