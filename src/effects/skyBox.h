#pragma once

#include "material/texture.h"
#include "material/shader.h"

class SkyBox
{
public:
	SkyBox();

	void draw(glm::mat4 view, glm::mat4 projection, std::shared_ptr<Shader> shader);
	
	void loadCubemap(const std::vector<std::filesystem::path>& faces);
	void createIrradianceMap();
	void createPrefilterMap();
	void createBRDFLUTTexture();

	uint32_t getCubemapTextureID() { return m_cubemapTextureID; }
	uint32_t getIrradianceMapID() { return m_irradianceMapID; }
	uint32_t getPrefilterMapID() { return m_prefilterMapID; }
	uint32_t getBRDFLUTTexture() { return m_brdfLUTTexture; }

	void convolveCubemap(std::shared_ptr<Shader> computeShader);
	void prefilterCubemap(std::shared_ptr<Shader> computeShader);
	void generateBRDFLUTTexture(std::shared_ptr<Shader> computeShader);

	void initData(const std::vector<std::filesystem::path>& faces);

private:
	uint32_t m_cubemapTextureID;
	uint32_t m_irradianceMapID;
	uint32_t m_prefilterMapID;
	uint32_t m_brdfLUTTexture;

	uint32_t m_vao, m_vbo;

};
