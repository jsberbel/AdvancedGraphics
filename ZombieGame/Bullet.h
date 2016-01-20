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
	int _damage;
	
	bool collideWithLevel(const std::vector<std::string> &lvlData);
public:
	explicit Bullet() = default;
	explicit Bullet(const glm::vec2 &position, const glm::vec2 &direction, float speed, int damage);
	~Bullet() = default;
	bool update(float deltaTime, const std::vector<std::string> &lvlData);
	void pushBatch(SerraEngine::SpriteBatch &spriteBatch);
	bool collideWithAgent(Agent* agent);
	
	inline int getDamage() const { return _damage; };
};

