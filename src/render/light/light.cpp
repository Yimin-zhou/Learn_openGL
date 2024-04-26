#include "light.h"

Light::Light(const LightType type, const glm::vec3& position, const glm::vec3& color, float intensity) :
	m_type(type),
	m_position(position),
	m_color(color),
	m_intensity(intensity),
	m_direction(0.0f),
	m_size(0.7f)
{
	m_rotation.x = glm::degrees(glm::asin(m_direction.y));
	m_rotation.y = glm::degrees(glm::atan(m_direction.x, m_direction.z));
	m_rotation.z = 0.0f;
}

const glm::vec3& Light::getPosition() const
{
	return m_position;
}

const glm::vec3& Light::getDirection() const
{
	return m_direction;
}

const glm::vec3& Light::getRotation() const
{
	return m_rotation;
}

void Light::setDirection(const glm::vec3& direction)
{
	m_direction = direction;
	m_rotation.x = glm::degrees(glm::asin(m_direction.y));
	m_rotation.y = glm::degrees(glm::atan(m_direction.x, m_direction.z));
	m_rotation.z = 0.0f;
}


void Light::setPosition(const glm::vec3& position)
{
	m_position = position;
}

void Light::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	m_direction.x = glm::cos(glm::radians(m_rotation.x)) * glm::sin(glm::radians(m_rotation.y));
	m_direction.y = glm::sin(glm::radians(m_rotation.x));
	m_direction.z = glm::cos(glm::radians(m_rotation.x)) * glm::cos(glm::radians(m_rotation.y));
}

const glm::vec3& Light::getColor() const
{
	return m_color;
}

void Light::setColor(const glm::vec3& color)
{
	m_color = color;
}

float Light::getIntensity() const
{
	return m_intensity;
}

void Light::setIntensity(float intensity)
{
	m_intensity = intensity;
}

void Light::accept(LightVisitor& visitor)
{

}
