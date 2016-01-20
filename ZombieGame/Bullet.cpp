#include "Bullet.h"
#include <SerraEngine\ResourceManager.h>
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

const int Bullet::BULLET_RADIUS = 5;

Bullet::Bullet(const glm::vec2 &position, const glm::vec2 &direction, float speed, int damage) :
	_position(position),
	_direction(direction),
	_speed(speed),
	_damage(damage) {}

bool Bullet::update(float deltaTime, const std::vector<std::string> &lvlData) {
	_position += _direction*_speed*deltaTime;
	return collideWithLevel(lvlData);
}

void Bullet::pushBatch(SerraEngine::SpriteBatch & spriteBatch) {
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = SerraEngine::ResourceManager::getTexture("Textures/circle.png").id;
	spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, SerraEngine::ColorRGBA8{75, 75, 75, 255});
}

bool Bullet::collideWithAgent(Agent * agent) {
	const float MIN_DISTANCE = Agent::AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(Agent::AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) return true;
	return false;
}

bool Bullet::collideWithLevel(const std::vector<std::string>& lvlData) {
	glm::ivec2 gridPosition;
	gridPosition.x = (int)floor(_position.x / (float)Level::TILE_WIDTH);
	gridPosition.y = (int)floor(_position.y / (float)Level::TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x >= (signed)lvlData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= (signed)lvlData.size()) return true;

	return (lvlData[gridPosition.y][gridPosition.x] != '.');
}
