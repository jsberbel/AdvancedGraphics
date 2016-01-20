#include "Gun.h"
#include <random>
#include <ctime>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(const std::string &name, int fireRate, int bulletPerShot, float spread, float bulletSpeed, int bulletDamage) :
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(bulletPerShot),
	_spread(spread),
	_bulletSpeed(bulletSpeed),
	_bulletDamage(bulletDamage),
	_frameCounter(0) {}

void Gun::update(bool isMouseDown, const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet>& bullets, float deltaTime) {
	_frameCounter += deltaTime;
	if (_frameCounter >= _fireRate && isMouseDown) {
		fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet>& bullets) {
	static std::mt19937 randomEngine((unsigned)time(nullptr));
	std::uniform_real_distribution<float> randRot(-_spread, _spread);

	for (int i = 0; i < _bulletsPerShot; i++) {
		glm::vec2 dir = glm::rotate(direction, glm::radians(randRot(randomEngine)));
		bullets.emplace_back(position, dir, _bulletSpeed, _bulletDamage);
	}
}
