#pragma once

#include "render/deferredRenderer.h"

class RoverRender : public DeferredRenderer
{
public:
	RoverRender() : DeferredRenderer() {}

	void init() override;
	void preprocess() override;
	void draw(glm::vec2 renderSize) override;

protected:
	// override
	void lightingPass() override;
	void forwardPass() override;

};