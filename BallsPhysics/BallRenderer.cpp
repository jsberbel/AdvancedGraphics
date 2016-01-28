#include "BallRenderer.h"

void BallRenderer::renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (m_program == nullptr) {
		m_program = std::make_unique<SerraEngine::GLSLManager>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->bind();

	auto textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	auto pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	spriteBatch.begin();
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (auto &b : balls) {
		const glm::vec4 destRect(b.position.x - b.radius, b.position.y - b.radius,
								 b.radius * 2.0f, b.radius * 2.0f);
		spriteBatch.pushBatch(destRect, uvRect, b.textureId, 0.0f, b.color);
	}
	spriteBatch.end();
	spriteBatch.renderBatches();

	m_program->unbind();
}

void MomentumBallRenderer::renderBalls(SerraEngine::SpriteBatch & spriteBatch, const std::vector<Ball>& balls, const glm::mat4 & projectionMatrix) {
	glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
	if (m_program == nullptr) {
		m_program = std::make_unique<SerraEngine::GLSLManager>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->bind();

	auto textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	auto pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	spriteBatch.begin();
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (auto &b : balls) {
		const glm::vec4 destRect(b.position.x - b.radius, b.position.y - b.radius,
			b.radius * 2.0f, b.radius * 2.0f);
		auto colorVal = static_cast<GLubyte>(glm::clamp(glm::length(b.velocity)*b.mass*12.0f, 0.0f, 255.0f));
		const SerraEngine::ColorRGBA8 color{ colorVal, colorVal, colorVal, colorVal };
		spriteBatch.pushBatch(destRect, uvRect, b.textureId, 0.0f, color);
	}
	spriteBatch.end();
	spriteBatch.renderBatches();

	m_program->unbind();
}

VelocityBallRenderer::VelocityBallRenderer(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight){
}

void VelocityBallRenderer::renderBalls(SerraEngine::SpriteBatch & spriteBatch, const std::vector<Ball>& balls, const glm::mat4 & projectionMatrix) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (m_program == nullptr) {
		m_program = std::make_unique<SerraEngine::GLSLManager>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->bind();

	auto textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	auto pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	spriteBatch.begin();
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (auto &b : balls) {
		const glm::vec4 destRect(b.position.x - b.radius, b.position.y - b.radius,
			b.radius * 2.0f, b.radius * 2.0f);
		const float mult = 100.0f;
		auto colorVal = static_cast<GLubyte>(glm::clamp(b.velocity.x*mult, 0.0f, 255.0f));
		const SerraEngine::ColorRGBA8 color{ 128, 
											 static_cast<GLubyte>((b.position.x / m_screenWidth)*255.0f), 
											 static_cast<GLubyte>((b.position.y / m_screenHeight)*255.0f), 
											 colorVal };
		spriteBatch.pushBatch(destRect, uvRect, b.textureId, 0.0f, color);
	}
	spriteBatch.end();
	spriteBatch.renderBatches();

	m_program->unbind();
}

TrippyBallRenderer::TrippyBallRenderer(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight) {
}

void TrippyBallRenderer::renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);

	// Lazily initialize the program
	if (m_program == nullptr) {
		m_program = std::make_unique<SerraEngine::GLSLManager>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}
	m_program->bind();

	spriteBatch.begin();

	// Make sure the shader uses texture 0
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Change these constants to get cool stuff
	float TIME_SPEED = 0.01f;
	float DIVISOR = 4.0f; // Increase to get more arms
	float SPIRAL_INTENSITY = 10.0f; // Increase to make it spiral more

	m_time += TIME_SPEED;

	// Render all the balls
	for (auto& ball : balls) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);
		SerraEngine::ColorRGBA8 color;

		// Get vector from center point
		glm::vec2 centerVec = ball.position - glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
		float centerDist = glm::length(centerVec);

		// Get angle from the horizontal
		float angle = static_cast<float>(atan2(centerVec.x, centerVec.y) / (3.1415926 / DIVISOR));
		// Move with time
		angle -= m_time;
		// Add the spiral
		angle += (centerDist / m_screenWidth) * SPIRAL_INTENSITY;

		color.r = static_cast<GLubyte>(angle * 255.0f);
		color.g = static_cast<GLubyte>(angle * 255.0f * cos(m_time));
		color.b = static_cast<GLubyte>(angle * 255.0f * sin(m_time));
		color.a = static_cast<GLubyte>(glm::clamp(1.0f - (centerDist / (m_screenWidth / 2.0f)), 0.0f, 1.0f) * 255.0f);
		spriteBatch.pushBatch(destRect, uvRect, ball.textureId, 0.0f, color);
	}

	spriteBatch.end();
	spriteBatch.renderBatches();

	m_program->unbind();
}
