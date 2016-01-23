#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"

namespace SerraEngine {

class Particle2D
{
	glm::vec2 m_position = glm::vec2(0.0f);
	glm::vec2 m_velocity = glm::vec2(0.0f);
	float m_life = 0.0f;
	float m_width = 0.0f;
	ColorRGBA8 m_color;
public:
	explicit Particle2D() = default;
	~Particle2D() = default;

	void update(float deltaTime);

	friend class ParticleBatch2D;
};

class ParticleBatch2D
{
	int m_maxParticles = 0;
	int m_lastFreeParticle = 0;
	float m_decayRate = 0.1f;
	Particle2D* m_particles = nullptr;
	GLuint m_idTexture;

	int findFreeParticle();
public:
	explicit ParticleBatch2D() = default;
	explicit ParticleBatch2D(int maxParticles, float decayRate, const GLuint &texture);
	~ParticleBatch2D();

	void init(int maxParticles, float decayRate, const GLuint &texture);
	void addParticle(const glm::vec2 &position, const glm::vec2 &velocity, float width, const ColorRGBA8 &color = ColorRGBA8(255, 255, 255, 255));

	void draw(SpriteBatch &spriteBatch) const;
	void update(float deltaTime) const;
};

}
