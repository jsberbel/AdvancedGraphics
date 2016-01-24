#include "Zombie.h"
#include "Human.h"
#include <SerraEngine/ResourceManager.h>

Zombie::Zombie(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 & color) :
	Agent(position, speed, color) {
	m_health = 150;
	m_textureID = SerraEngine::ResourceManager::getTexture("Textures/zombie.png").id;
	m_startDir = m_direction;
	m_endDir = m_direction;
}

void Zombie::update(float deltaTime, const std::vector<std::string>& lvlData, const std::vector<Human*>& humans) {
	auto dirCol = false;
	auto prevPos = m_position;

	auto closestHuman = getNearestHuman(humans);
	m_closestHumanPos = closestHuman->getPosition();

	if (m_countDir < 1.0f) m_direction = Nlerp(m_startDir, m_endDir, m_countDir), m_countDir += 0.01f;
	
	if (closestHuman != nullptr) {
		m_position += m_direction*m_speed*deltaTime;
		if (m_closestHumanPos != m_prevHumanPos) {
			m_startDir = m_direction;
			m_endDir = glm::normalize(m_closestHumanPos - m_position);
			m_countDir = 0.0f;
			m_prevHumanPos = m_closestHumanPos;
			printf("%f\n", m_direction.x);
		}
	}

	if (m_position.y > prevPos.y) dirCol = true;
	collideWithLevel(lvlData, dirCol);
}

Human* Zombie::getNearestHuman(const std::vector<Human*>& humans) const
{
	Human* closestHuman = nullptr;
	auto smallestDistance = 999999.0f;
	for (auto h : humans) {
		auto distVec = h->getPosition() - m_position;
		auto distance = glm::length(distVec);
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = h;
		}
	}
	return closestHuman;
}

