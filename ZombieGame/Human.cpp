#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

const int Human::MAX_FRAMES = 30;

Human::Human(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color) :
	_frames(0),
	Agent(position, speed, color)
{
	_health = 20;

	static std::mt19937 randomEngine((unsigned)time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_direction = { randDir(randomEngine), randDir(randomEngine) };
	if (glm::length(_direction)) _direction = { 1.0f, 0.0f };
	_direction = glm::normalize(_direction);
}

void Human::update(float deltaTime, const std::vector<std::string>& lvlData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	static bool dirCol = false;
	glm::vec2 prevPos = _position;
	_position += _direction*_speed*deltaTime;

	static std::mt19937 randomEngine((unsigned)time(nullptr));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);

	if(_frames >= MAX_FRAMES) _direction = glm::rotate(_direction, glm::radians(randRot(randomEngine))), _frames = 0;
	else _frames++;

	if (_position.y > prevPos.y) dirCol = true;
	else dirCol = false;
	if (collideWithLevel(lvlData, dirCol)) _direction = glm::rotate(_direction, randRot(randomEngine));
}
