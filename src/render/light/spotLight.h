#pragma once

#include "light.h"

class SpotLight : public Light 
{
public:
	SpotLight(const glm::vec3& position, const glm::vec3& direction, 
		const glm::vec3& color, float intensity, float constant, float linear, 
		float quadratic, float innerCutoff, float outerCutoff);

	float getConstant() const;

	void setConstant(float constant);

	float getLinear() const;

	void setLinear(float linear);

	float getQuadratic() const;

	void setQuadratic(float quadratic);

	float getInnerCutoff() const;

	void setInnerCutoff(float innerCutoff);

	float getOuterCutoff() const;

	void setOuterCutoff(float outerCutoff);

	void accept(LightVisitor& visitor) override { visitor.visit(*this); }

private:
	glm::vec3 m_direction;
	float m_innerCutoff;
	float m_outerCutoff;
	float m_constant;
	float m_linear;
	float m_quadratic;
};

