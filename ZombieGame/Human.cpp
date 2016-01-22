#include "Human.h"
#include <random>
#include <ctime>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>

Human::Human(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color) :
	Agent(position, speed, color),
	m_frames(0)
{
	_health = 20;

	static std::mt19937 randomEngine(static_cast<unsigned>(time(nullptr)));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_direction = { randDir(randomEngine), randDir(randomEngine) };
	if (glm::length(m_direction)) m_direction = { 1.0f, 0.0f };
	m_direction = glm::normalize(m_direction);
}

void Human::update(float deltaTime, const std::vector<std::string>& lvlData, const std::vector<Human*>& humans) {
	static bool dirCol;
	auto prevPos = _position;
	_position += m_direction*_speed*deltaTime;

	static std::mt19937 randomEngine(static_cast<unsigned>(time(nullptr)));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);

	if(m_frames >= MAX_FRAMES) m_direction = glm::rotate(m_direction, glm::radians(randRot(randomEngine))), m_frames = 0;
	else m_frames++;

	if (_position.y > prevPos.y) dirCol = true;
	else dirCol = false;
	if (collideWithLevel(lvlData, dirCol)) m_direction = glm::rotate(m_direction, randRot(randomEngine));
}
