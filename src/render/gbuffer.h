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
};