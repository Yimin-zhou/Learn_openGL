#include "spotLight.h"

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, 
	const glm::vec3& color, float intensity, 
	float constant, float linear, float quadratic, float innerCutoff, float outerCutoff) : 
	Light(LightType::SPOT, position, color, intensity),
	m_direction(direction), m_innerCutoff(innerCutoff), m_outerCutoff(outerCutoff),
	m_constant(constant), m_linear(linear), m_quadratic(quadratic)
{
}

float SpotLight::getConstant() const
{
	return m_constant;
}

void SpotLight::setConstant(float constant)
{
	m_constant = constant;
}

float SpotLight::getLinear() const
{
	return m_linear;
}

void SpotLight::setLinear(float linear)
{
	m_linear = linear;
}

float SpotLight::getQuadratic() const
{
	return m_quadratic;
}

void SpotLight::setQuadratic(float quadratic)
{
	m_quadratic = quadratic;
}

float SpotLight::getInnerCutoff() const
{
	return m_innerCutoff;
}

void SpotLight::setInnerCutoff(float innerCutoff)
{
	m_innerCutoff = innerCutoff;
}

float SpotLight::getOuterCutoff() const
{
	return m_outerCutoff;
}

void SpotLight::setOuterCutoff(float outerCutoff)
{
	m_outerCutoff = outerCutoff;
}

