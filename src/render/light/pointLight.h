#pragma once

#include "light.h"

class PointLight : public Light 
{
public:
    PointLight(const glm::vec3& position, const glm::vec3& color, float intensity,
        float constant, float linear, float quadratic, float radius);

    float getConstant() const;
    void setConstant(float constant);

    float getLinear() const;
    void setLinear(float linear);

    float getQuadratic() const;
    void setQuadratic(float quadratic);

    float getRadius() const;
    void setRadius(float radius);

    void accept(LightVisitor& visitor) override;

private:
    float m_constant;
    float m_linear;
    float m_quadratic;
    float m_radius; // ????
};
