#pragma once
#include <vector>
#include "Bullet.h"

class Gun
{
	std::string m_name;
	int m_fireRate;			// Rate per frames
	int m_bulletsPerShot;	// How many bullets fired at x time
	float m_spread;			// Accuracy
	float m_bulletSpeed;
	int m_bulletDamage;
	float m_frameCounter;

	void fire(const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet> &bullets);
public:
	explicit Gun() = default;
	explicit Gun(const std::string &name, int fireRate, int bulletPerShot, float spread, float _bulletSpeed, int _bulletDamage);
	~Gun() = default;
	void update(bool isMouseDown, const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet>& bullets, float deltaTime);
	
};

