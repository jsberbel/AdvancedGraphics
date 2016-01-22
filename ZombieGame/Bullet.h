#pragma once
#include <SerraEngine/SpriteBatch.h>

class Human;
class Zombie;
class Agent;

class Bullet
{
	static const int BULLET_RADIUS;

	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_speed;
	int m_damage;
	
	bool collideWithLevel(const std::vector<std::string> &lvlData) const;
public:
	explicit Bullet() = default;
	explicit Bullet(const glm::vec2 &position, const glm::vec2 &direction, float speed, int damage);
	~Bullet() = default;
	bool update(float deltaTime, const std::vector<std::string> &lvlData);
	void pushBatch(SerraEngine::SpriteBatch &spriteBatch) const;
	bool collideWithAgent(Agent* agent) const;
	
	int getDamage() const { return m_damage; };
};

