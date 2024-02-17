#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& location, const glm::vec3& color, float intensity) :
	Light(LightType::DIRECTIONAL, location, color, intensity), // Directional light has no position
	m_direction(location)
{

}