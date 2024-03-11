#pragma once

#include <stdint.h>

struct GBuffer
{
	uint32_t fbo;
	uint32_t worldPosTex;
	uint32_t worldNormalTex;
	uint32_t albedoTex;
	uint32_t roughnessMetalnessAoTex;
	uint32_t emissionTex;
	uint32_t depthStencilTex;

public:
	uint32_t getWorldPosTex() { return worldPosTex; }
	uint32_t getWorldNormalTex() { return worldNormalTex; }
	uint32_t getAlbedoTex() { return albedoTex; }
	uint32_t getRoughnessMetalnessAoTex() { return roughnessMetalnessAoTex; }
	uint32_t getEmissionTex() { return emissionTex; }
	uint32_t getDepthStencilTex() { return depthStencilTex; }
};