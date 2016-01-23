#include "ParticleBatch2D.h"

namespace SerraEngine {

void Particle2D::update(float deltaTime) {
	m_position += m_velocity*deltaTime;
}

ParticleBatch2D::ParticleBatch2D(int maxParticles, float decayRate, const GLuint &texture) :
	m_maxParticles(maxParticles),
	m_decayRate(decayRate),
	m_particles(new Particle2D[m_maxParticles]),
	m_idTexture(texture) {}

ParticleBatch2D::~ParticleBatch2D() {
	delete[] m_particles;
}

void ParticleBatch2D::init(int maxParticles, float decayRate, const GLuint &texture) {
	m_maxParticles = maxParticles;
	m_decayRate = decayRate;
	m_particles = new Particle2D[m_maxParticles];
	m_idTexture = texture;
}

int ParticleBatch2D::findFreeParticle() {
	for (auto i = m_lastFreeParticle; i < m_maxParticles; i++) {
		if (m_particles[i].m_life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	for (auto i = 0; i < m_lastFreeParticle; i++) {
		if (m_particles[i].m_life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	return 0;
}

void ParticleBatch2D::addParticle(const glm::vec2 & position, const glm::vec2 & velocity, float width, const ColorRGBA8 & color) {
	auto particleIndex = findFreeParticle();
	auto &p = m_particles[particleIndex];
	p.m_position = position;
	p.m_velocity = velocity;
	p.m_life = 1.0f;
	p.m_width = width;
	p.m_color = color;
	
}

void ParticleBatch2D::draw(SpriteBatch &spriteBatch) const {
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (auto i = 0; i < m_maxParticles; i++) {
		auto &p = m_particles[i];
		if (p.m_life > 0.0f) {
			glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);
			spriteBatch.pushBatch(destRect, uvRect, m_idTexture, 0.0f, p.m_color);
		}
	}
}

void ParticleBatch2D::update(float deltaTime) const {
	for (auto i = 0; i < m_maxParticles; i++) {
		if (m_particles[i].m_life > 0.0f) {
			m_particles[i].update(deltaTime);
			m_particles[i].m_life -= m_decayRate*deltaTime;
		}
	}
}

}