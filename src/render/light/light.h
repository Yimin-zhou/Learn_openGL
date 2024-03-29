#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

enum class LightType
{
	DIRECTIONAL,
	POINT,
	SPOT
};

class DirectionalLight;
class PointLight;
class SpotLight;

class LightVisitor 
{
public:
	virtual void visit(DirectionalLight& light) = 0;
	virtual void visit(PointLight& light) = 0;
	virtual void visit(SpotLight& light) = 0;

	virtual ~LightVisitor() = default;
};


class Light 
{
public:
	Light(const LightType type, const glm::vec3& position, const glm::vec3& color, float intensity);

	virtual ~Light() = default;

	LightType getType() const { return m_type;}

	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;
	const glm::vec3& getRotation() const;

	void setDirection(const glm::vec3& direction);
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);

	const glm::vec3& getColor() const;

	void setColor(const glm::vec3& color);

	float getIntensity() const;

	void setIntensity(float intensity);

	void setLightSize(float size) { m_lightSize = size; }
	const float getLightSize() const { return m_lightSize; }

	virtual void accept(LightVisitor& visitor) = 0;

private:
	LightType m_type;

	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_rotation;
	glm::vec3 m_color;
	float m_intensity;

	float m_lightSize;
};
