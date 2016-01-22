#include "Bullet.h"
#include <SerraEngine\ResourceManager.h>
#include "Human.h"
#include "Level.h"

const int Bullet::BULLET_RADIUS = 5;

Bullet::Bullet(const glm::vec2 &position, const glm::vec2 &direction, float speed, int damage) :
	m_position(position),
	m_direction(direction),
	m_speed(speed),
	m_damage(damage) {}

bool Bullet::update(float deltaTime, const std::vector<std::string> &lvlData) {
	m_position += m_direction*m_speed*deltaTime;
	return collideWithLevel(lvlData);
}

void Bullet::pushBatch(SerraEngine::SpriteBatch & spriteBatch) const
{
	glm::vec4 destRect(m_position.x + BULLET_RADIUS, m_position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = SerraEngine::ResourceManager::getTexture("Textures/circle.png").id;
	spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, SerraEngine::ColorRGBA8{75, 75, 75, 255});
}

bool Bullet::collideWithAgent(Agent * agent) const
{
	const auto MIN_DISTANCE = Agent::AGENT_RADIUS + BULLET_RADIUS;

	auto centerPosA = m_position;
	auto centerPosB = agent->getPosition() + glm::vec2(Agent::AGENT_RADIUS);
	auto distVec = centerPosA - centerPosB;

	auto distance = glm::length(distVec);
	auto collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) return true;
	return false;
}

bool Bullet::collideWithLevel(const std::vector<std::string>& lvlData) const
{
	glm::ivec2 gridPosition;
	gridPosition.x = static_cast<int>(floor(m_position.x / static_cast<float>(Level::TILE_WIDTH)));
	gridPosition.y = static_cast<int>(floor(m_position.y / static_cast<float>(Level::TILE_WIDTH)));

	if (gridPosition.x < 0 || gridPosition.x >= static_cast<signed>(lvlData[0].size()) ||
		gridPosition.y < 0 || gridPosition.y >= static_cast<signed>(lvlData.size())) return true;

	return (lvlData[gridPosition.y][gridPosition.x] != '.');
}
