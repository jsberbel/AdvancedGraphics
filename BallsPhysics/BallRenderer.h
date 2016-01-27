#pragma once

#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/GLSLManager.h>
#include "Ball.h"
#include <memory>

class BallRenderer {
protected:
	std::unique_ptr<SerraEngine::GLSLManager> m_program = nullptr;
public:
	virtual void renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix);
};

class MomentumBallRenderer : public BallRenderer {
public:
	virtual void renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
};
