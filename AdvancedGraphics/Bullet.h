#pragma once
#include <glm/glm.hpp>
#include <SerraEngine/SpriteBatch.h>

class Bullet
{
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
	int _lifeTime;
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float sp, int lifeTime);
	~Bullet();
	void draw(SerraEngine::SpriteBatch &spriteBatch);
	bool update();
};