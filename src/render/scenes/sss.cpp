#include "sss.h"

void SSSRenderer::init()
{
	m_models.push_back(Model(modelPath, ModelType::OPAQUE));

	m_shaderManager.buildShader(ShaderName::GBUFFER, "shader/deferred/geo_vert.glsl", "shader/deferred/geo_frag.glsl");
	m_shaderManager.buildShader(ShaderName::PBR, "shader/deferred/pbr/deferred_vert.glsl", "shader/deferred/pbr/deferred_pbr_frag.glsl");
	m_shaderManager.buildShader(ShaderName::SKYBOX, "shader/skybox/skybox_vert.glsl", "shader/skybox/skybox_frag.glsl");
	m_shaderManager.buildShader(ShaderName::SHADOW, "shader/shadow/shadow_vert.glsl");
	m_shaderManager.buildComputeShader(ShaderName::CONVOLUTION, "shader/pbr/convolve_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PREFILTER, "shader/pbr/prefilter_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PRE_BRDF, "shader/pbr/precompute_brdf.glsl");

	// light
	{
		std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f);
		directionalLight->setDirection({ 0.0f, 1.0f, 1.0f });

		m_lightManager.addLight(directionalLight);
	}

	// create framebuffers
	createGbuffer();
	createLightingFramebuffer();
}

void SSSRenderer::preprocess()
{
	// skybox
	m_skyBox.initData(std::vector<std::filesystem::path>{
			"res/textures/skybox/studio/px.png",
			"res/textures/skybox/studio/nx.png",
			"res/textures/skybox/studio/py.png",
			"res/textures/skybox/studio/ny.png",
			"res/textures/skybox/studio/pz.png",
			"res/textures/skybox/studio/nz.png"
	});
	m_skyBox.convolveCubemap(m_shaderManager.getShader(ShaderName::CONVOLUTION));
	m_skyBox.prefilterCubemap(m_shaderManager.getShader(ShaderName::PREFILTER));
	m_skyBox.generateBRDFLUTTexture(m_shaderManager.getShader(ShaderName::PRE_BRDF));

	// shadow
	m_shadow = Shadow(2048, 2048);
}

void SSSRenderer::draw(glm::vec2 renderSize)
{
	DeferredRenderer::draw(renderSize);
}

void SSSRenderer::lightingPass()
{
	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightingFBO);
	glViewport(0, 0, m_renderSize.x, m_renderSize.y);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	std::shared_ptr<Shader> shader = m_shaderManager.getShader(ShaderName::PBR);
	shader->bind();

	shader->setUniform("cameraPos", m_camera.getPosition());

	// bind gbuffer textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.worldPosTex);
	shader->setUniform("gWorldPos", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.worldNormalTex);
	shader->setUniform("gWorldNormal", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.albedoTex);
	shader->setUniform("gAlbedo", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.roughnessMetalnessAoTex);
	shader->setUniform("gRoughnessMetalnessAo", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.emissionTex);
	shader->setUniform("gEmission", 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_gBuffer.depthStencilTex);
	shader->setUniform("gDepth", 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBox.getIrradianceMapID());
	shader->setUniform("irradianceMap", 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBox.getPrefilterMapID());
	shader->setUniform("prefilterMap", 7);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_skyBox.getBRDFLUTTexture());
	shader->setUniform("brdfLUT", 8);

	// bind shadow map
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_shadow.getShadowMap());
	shader->setUniform("shadowMap", 9);
	shader->setUniform("lightSpaceMatrix", m_shadow.getLightSpaceMatrix());

	// bind light data
	m_lightManager.updateShader(m_shaderManager.getShader(ShaderName::PBR));

	// draw quad
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}
