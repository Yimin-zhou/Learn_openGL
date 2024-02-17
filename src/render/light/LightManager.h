#pragma once

#include "light.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "material/shader.h"

class ShaderLightUpdater : public LightVisitor 
{
public:
	ShaderLightUpdater(std::shared_ptr<Shader> shader) : m_shader(shader) {}

	void visit(DirectionalLight& light) override;

	void visit(PointLight& light) override;

	void visit(SpotLight& light) override;

private:
	std::shared_ptr<Shader> m_shader;
	int m_pointCount = 0;
	int m_spotCount = 0;
};

class LightManager
{
public:
	LightManager();

	void addLight(const std::shared_ptr<DirectionalLight> light);
	void addLight(const std::shared_ptr<PointLight> light);
	void addLight(const std::shared_ptr<SpotLight> light);

	const std::shared_ptr<DirectionalLight> getDirectionalLight() const { return m_directionalLight; }
	const std::vector<std::shared_ptr<PointLight>>& getPointLights() const { return m_pointLights; }
	const std::vector<std::shared_ptr<SpotLight>>& getSpotLights() const { return m_spotLights; }

	void updateShader(std::shared_ptr<Shader> shader);

private:
	std::shared_ptr<DirectionalLight> m_directionalLight;
	std::vector<std::shared_ptr<PointLight>> m_pointLights;
	std::vector<std::shared_ptr<SpotLight>> m_spotLights;
};
