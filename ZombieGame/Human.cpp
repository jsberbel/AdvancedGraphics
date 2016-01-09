#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

const int Human::MAX_FRAMES = 30;

Human::Human(const glm::vec2 & position, float speed, const SerraEngine::Color& color) :
	_frames(0),
	Agent(position, speed, color) 
{
	static std::mt19937 randomEngine((unsigned)time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_direction = { randDir(randomEngine), randDir(randomEngine) };
	if (_direction.length() == 0) _direction = { 1.0f, 0.0f };
	_direction = glm::normalize(_direction);
}

void Human::update() {
	static bool dirCol = false;
	glm::vec2 prevPos = _position;
	_position += _direction*_speed;

	static const float PI = 3.14159265359f;
	static std::mt19937 randomEngine((unsigned)time(nullptr));
	static std::uniform_real_distribution<float> randRot(-40.0f * PI / 180, 40.0f * PI / 180);

	if(_frames >= MAX_FRAMES) _direction = glm::rotate(_direction, randRot(randomEngine)), _frames = 0;
	else _frames++;

	if (_position.y > prevPos.y) dirCol = true;
	else dirCol = false;
	if (collideWithLevel(dirCol)) _direction = glm::rotate(_direction, randRot(randomEngine));
}
