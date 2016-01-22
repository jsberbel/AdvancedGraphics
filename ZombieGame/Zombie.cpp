#include "Zombie.h"
#include "Human.h"

Zombie::Zombie(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 & color) :
	Agent(position, speed, color) {
	_health = 150;
}

void Zombie::update(float deltaTime, const std::vector<std::string>& lvlData, const std::vector<Human*>& humans) {
	static auto dirCol = false;
	auto prevPos = _position;

	auto closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction*_speed*deltaTime;
	}

	if (_position.y > prevPos.y) dirCol = true;
	else dirCol = false;
	collideWithLevel(lvlData, dirCol);
}

Human* Zombie::getNearestHuman(const std::vector<Human*>& humans) const
{
	Human* closestHuman = nullptr;
	auto smallestDistance = 999999.0f;
	for (auto h : humans) {
		auto distVec = h->getPosition() - _position;
		auto distance = glm::length(distVec);
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = h;
		}
	}
	return closestHuman;
}

