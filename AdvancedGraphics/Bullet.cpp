#include "Bullet.h"
#include <SerraEngine/ResourceManager.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float sp, int lifeTime) :
	_position(pos),
	_direction(dir),
	_speed(sp),
	_lifeTime(lifeTime) {}


Bullet::~Bullet()
{
}

void Bullet::draw(SerraEngine::SpriteBatch & spriteBatch) {
	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static SerraEngine::GLTexture tex = SerraEngine::ResourceManager::getTexture("./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	spriteBatch.draw(posAndSize, uv, tex.id, 0.0f, SerraEngine::Color(255, 255, 255, 255));
}

bool Bullet::update() {
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) return true;
	return false;
}