#pragma once
#include "Human.h"
#include "Bullet.h"
#include <SerraEngine/InputManager.h>
#include <SerraEngine/Camera2D.h>

#define PLAYER_COLOR { 0,0, 185, 255 }
#define GUN_EMPTY -1

class Gun;

class Player : public Human
{
	SerraEngine::InputManager &_inputManager;
	SerraEngine::Camera2D &_camera;
	std::vector<Bullet> &_bullets;
	std::vector<Gun*> _guns;
	int _currentGun;
public:
	Player() = default;
	Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager, SerraEngine::Camera2D &camera, std::vector<Bullet> &bullets);
	~Player();
	void update(float deltaTime = 0,
				const std::vector<std::string>& lvlData = std::vector<std::string>(),
				std::vector<Human*>& humans = std::vector<Human*>(),
				std::vector<Zombie*>& zombies = std::vector<Zombie*>()) override;
	void addGun(Gun* gun);
};

