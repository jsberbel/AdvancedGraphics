#pragma once
#include <SerraEngine/SpriteBatch.h>
#include <list>
#include <glm/glm.hpp>

class Human;
class Zombie;
class Agent;

class Bullet
{
	static const int BULLET_RADIUS;

	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
	float _damage;
	
	bool collideWithLevel(const std::vector<std::string> &lvlData);
public:
	Bullet() = default;
	Bullet(const glm::vec2 &position, const glm::vec2 &direction, float speed, float damage);
	~Bullet() = default;
	bool update(const std::vector<std::string> &lvlData);
	void pushBatch(SerraEngine::SpriteBatch &spriteBatch);
	bool collideWithAgent(Agent* agent);
	
	inline float getDamage() const { return _damage; };
};

