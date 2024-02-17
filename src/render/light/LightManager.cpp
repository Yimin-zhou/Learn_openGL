#include "LightManager.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"

LightManager::LightManager()
{

}

void LightManager::addLight(const std::shared_ptr<DirectionalLight> light)
{
	m_directionalLight = light;
}


void LightManager::addLight(const std::shared_ptr<PointLight> light)
{
	m_pointLights.push_back(light);
}

void LightManager::addLight(const std::shared_ptr<SpotLight> light)
{
	m_spotLights.push_back(light);
}

void LightManager::updateShader(std::shared_ptr<Shader> shader)
{
	ShaderLightUpdater lightUpdater(shader);

	m_directionalLight->accept(lightUpdater);

	for (auto& light : m_pointLights)
	{
		light->accept(lightUpdater);
	}

	for (auto& light : m_spotLights)
	{
		light->accept(lightUpdater);
	}
}

void ShaderLightUpdater::visit(DirectionalLight& light)
{
	m_shader->setUniform("directionalLight.direction", light.getDirection());
	m_shader->setUniform("directionalLight.color", light.getColor());
	m_shader->setUniform("directionalLight.intensity", light.getIntensity());
}

void ShaderLightUpdater::visit(PointLight& light)
{
	m_shader->setUniform("pointLights[" + std::to_string(m_pointCount) + "].position", light.getPosition());
	m_shader->setUniform("pointLights[" + std::to_string(m_pointCount) + "].color", light.getColor());
	m_shader->setUniform("pointLights[" + std::to_string(m_pointCount) + "].intensity", light.getIntensity());
	m_shader->setUniform("pointLights[" + std::to_string(m_pointCount) + "].constant", light.getConstant());
	m_shader->setUniform("pointLights[" + std::to_string(m_pointCount) + "].linear", light.getLinear());
	m_shader->setUniform("pointLights[" + std::to_string(m_pointCount) + "].quadratic", light.getQuadratic());
	m_pointCount++;
	m_shader->setUniform("activePointLights", m_pointCount);
}

void ShaderLightUpdater::visit(SpotLight& light)
{
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].position", light.getPosition());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].direction", light.getDirection());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].color", light.getColor());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].intensity", light.getIntensity());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].constant", light.getConstant());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].linear", light.getLinear());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].quadratic", light.getQuadratic());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].cutOff", light.getInnerCutoff());
	m_shader->setUniform("spotLights[" + std::to_string(m_spotCount) + "].outerCutOff", light.getOuterCutoff());
	m_spotCount++;
}
