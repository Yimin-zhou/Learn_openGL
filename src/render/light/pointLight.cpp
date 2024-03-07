
#include "pointLight.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float intensity,
	float constant, float linear, float quadratic, float radius) :
	Light(LightType::POINT, position, color, intensity),
	m_constant(constant), m_linear(linear),
	m_quadratic(quadratic), m_radius(radius)
{

}

float PointLight::getConstant() const
{
	return m_constant;
}

void PointLight::setConstant(float constant)
{
	m_constant = constant;
}

float PointLight::getLinear() const
{
	return m_linear;
}

void PointLight::setLinear(float linear)
{
	m_linear = linear;
}

float PointLight::getQuadratic() const
{
	return m_quadratic;
}

void PointLight::setQuadratic(float quadratic)
{
	m_quadratic = quadratic;
}

float PointLight::getRadius() const
{
	return m_radius;
}

void PointLight::setRadius(float radius)
{
	m_radius = radius;
}

void PointLight::accept(LightVisitor& visitor)
{
	visitor.visit(*this);
}

