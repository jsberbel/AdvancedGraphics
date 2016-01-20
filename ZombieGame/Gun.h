#pragma once
#include <string>
#include <vector>
#include "Bullet.h"

class Gun
{
	std::string _name;
	int _fireRate;			// Rate per frames
	int _bulletsPerShot;	// How many bullets fired at x time
	float _spread;			// Accuracy
	float _bulletSpeed;
	int _bulletDamage;
	float _frameCounter;

	void fire(const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet> &bullets);
public:
	explicit Gun() = default;
	explicit Gun(const std::string &name, int fireRate, int bulletPerShot, float spread, float _bulletSpeed, int _bulletDamage);
	~Gun() = default;
	void update(bool isMouseDown, const glm::vec2 & position, const glm::vec2 & direction, std::vector<Bullet>& bullets, float deltaTime);
	
};

