#include "BallRenderer.h"

void BallRenderer::renderBalls(SerraEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {
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
	for (auto b : balls) {
		const glm::vec4 destRect(b.position.x - b.radius, b.position.y - b.radius,
								 b.radius * 2.0f, b.radius * 2.0f);
		spriteBatch.pushBatch(destRect, uvRect, b.textureId, 0.0f, b.color);
	}
	spriteBatch.end();
	spriteBatch.renderBatches();

	m_program->unbind();
}

void MomentumBallRenderer::renderBalls(SerraEngine::SpriteBatch & spriteBatch, const std::vector<Ball>& balls, const glm::mat4 & projectionMatrix) {
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
	for (auto b : balls) {
		const glm::vec4 destRect(b.position.x - b.radius, b.position.y - b.radius,
			b.radius * 2.0f, b.radius * 2.0f);
		auto colorVal = static_cast<GLubyte>(glm::clamp(glm::length(b.velocity)*b.mass, 0.0f, 255.0f));
		const SerraEngine::ColorRGBA8 color{ colorVal, colorVal, colorVal, colorVal };
		spriteBatch.pushBatch(destRect, uvRect, b.textureId, 0.0f, color);
	}
	spriteBatch.end();
	spriteBatch.renderBatches();

	m_program->unbind();
}
