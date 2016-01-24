#include "Human.h"
#include <random>
#include <ctime>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <SerraEngine/ResourceManager.h>

Human::Human(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color) :
	Agent(position, speed, color),
	m_frames(0)
{
	m_health = 20;
	m_textureID = SerraEngine::ResourceManager::getTexture("Textures/human.png").id;

	m_startDir = m_direction;
	m_endDir = m_direction;

	static std::mt19937 randomEngine(static_cast<unsigned>(time(nullptr)));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_direction = { randDir(randomEngine), randDir(randomEngine) };
	if (glm::length(m_direction)) m_direction = { 1.0f, 0.0f };
	m_direction = glm::normalize(m_direction);
}

void Human::update(float deltaTime, const std::vector<std::string>& lvlData, const std::vector<Human*>& humans) {
	auto dirCol = false;
	auto prevPos = m_position;
	static auto collided = false;

	m_position += m_direction*m_speed*deltaTime;

	static std::mt19937 randomEngine(static_cast<unsigned>(time(nullptr)));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);

	if (m_position.y > prevPos.y) dirCol = true;

	if (collideWithLevel(lvlData, dirCol)) collided = true;
	if (m_countDir < 1.0f) m_direction = Nlerp(m_startDir, m_endDir, m_countDir), m_countDir += 0.01f;
	if (m_frames >= MAX_FRAMES) {
		m_startDir = m_direction;
		m_endDir = glm::rotate(m_direction, glm::radians(randRot(randomEngine)));
		m_frames = 0;
		m_countDir = 0.0f;
		if (collided) m_endDir = -m_endDir, collided = false;
	}
	else m_frames++;
}
