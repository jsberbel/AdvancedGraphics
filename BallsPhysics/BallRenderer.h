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

class VelocityBallRenderer : public BallRenderer {
	int m_screenWidth;
	int m_screenHeight;
public:
	VelocityBallRenderer(int screenWidth, int screenHeight);
	virtual void renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
};

class TrippyBallRenderer : public BallRenderer {
public:
	TrippyBallRenderer(int screenWidth, int screenHeight);
	virtual void renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
private:
	int m_screenWidth;
	int m_screenHeight;
	float m_time = 0.0f;
};