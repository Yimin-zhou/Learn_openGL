#include "defaultSence.h"

DefaultScene::DefaultScene()
{
	m_modelPath = "res/models/spaceman/spaceman.obj";
}

void DefaultScene::init()
{
	// 1. load models
	m_models.push_back(Model(m_modelPath, ModelType::OPAQUE));

	// 2. load shaders
	m_shaderManager.buildShader(ShaderName::GBUFFER, "shader/deferred/geo_vert.glsl", "shader/deferred/geo_frag.glsl");
	m_shaderManager.buildShader(ShaderName::PBR, "shader/deferred/pbr/deferred_vert.glsl", "shader/deferred/pbr/deferred_pbr_frag.glsl");
	m_shaderManager.buildShader(ShaderName::SKYBOX, "shader/skybox/skybox_vert.glsl", "shader/skybox/skybox_frag.glsl");
	m_shaderManager.buildShader(ShaderName::SHADOW, "shader/shadow/shadow_vert.glsl");
	m_shaderManager.buildComputeShader(ShaderName::CONVOLUTION, "shader/pbr/convolve_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PREFILTER, "shader/pbr/prefilter_cubemap.glsl");
	m_shaderManager.buildComputeShader(ShaderName::PRE_BRDF, "shader/pbr/precompute_brdf.glsl");

	// 3. set up lights
	{
		std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f);
		directionalLight->setDirection({ 0.0f, 1.0f, 1.0f });

		m_lightManager.addLight(directionalLight);
	}
}

void DefaultScene::preprocess()
{
	// skybox
	m_skyBox.initData(std::vector<std::filesystem::path>{
		"res/textures/skybox/pure/px.png",
			"res/textures/skybox/pure/nx.png",
			"res/textures/skybox/pure/py.png",
			"res/textures/skybox/pure/ny.png",
			"res/textures/skybox/pure/pz.png",
			"res/textures/skybox/pure/nz.png"
	});
	m_skyBox.convolveCubemap(m_shaderManager.getShader(ShaderName::CONVOLUTION));
	m_skyBox.prefilterCubemap(m_shaderManager.getShader(ShaderName::PREFILTER));
	m_skyBox.generateBRDFLUTTexture(m_shaderManager.getShader(ShaderName::PRE_BRDF));
}

void DefaultScene::update(std::shared_ptr<Window> window, float deltaTime)
{
	m_camera.update(window, deltaTime);
}

