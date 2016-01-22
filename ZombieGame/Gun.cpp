#include "Gun.h"
#include <random>
#include <ctime>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(const std::string &name, int fireRate, int bulletPerShot, float spread, float bulletSpeed, int bulletDamage) :
	m_name(name),
	m_fireRate(fireRate),
	m_bulletsPerShot(bulletPerShot),
	m_spread(spread),
	m_bulletSpeed(bulletSpeed),
	m_bulletDamage(bulletDamage),
	m_frameCounter(0) {}

void Gun::update(bool isMouseDown, const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet>& bullets, float deltaTime) {
	m_frameCounter += deltaTime;
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		fire(position, direction, bullets);
		m_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet>& bullets) {
	static std::mt19937 randomEngine(static_cast<unsigned>(time(nullptr)));
	std::uniform_real_distribution<float> randRot(-m_spread, m_spread);

	for (int i = 0; i < m_bulletsPerShot; i++) {
		glm::vec2 dir = glm::rotate(direction, glm::radians(randRot(randomEngine)));
		bullets.emplace_back(position, dir, m_bulletSpeed, m_bulletDamage);
	}
}
