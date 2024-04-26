#pragma once

#include "render/deferredRenderer.h"

class SSSRenderer : public DeferredRenderer
{
public:
	SSSRenderer() : DeferredRenderer() {}

	void init() override;
	void preprocess() override;
	void draw(glm::vec2 renderSize) override;

protected:
	void lightingPass() override;

	// SSS
};