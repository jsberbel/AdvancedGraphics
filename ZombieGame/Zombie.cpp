#include "Zombie.h"
#include "Human.h"

Zombie::Zombie(const glm::vec2 & position, float speed, const SerraEngine::Color & color) :
	Agent(position, speed, color) {
	_health = 150;
}

void Zombie::update(const std::vector<std::string>& lvlData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	static bool dirCol = false;
	glm::vec2 prevPos = _position;
	
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction*_speed;
	}

	if (_position.y > prevPos.y) dirCol = true;
	else dirCol = false;
	collideWithLevel(lvlData, dirCol);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = 999999.0f;
	for (auto h : humans) {
		glm::vec2 distVec = h->getPosition() - _position;
		float distance = glm::length(distVec);
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = h;
		}
	}
	return closestHuman;
}

