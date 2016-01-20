#pragma once
#include "Human.h"
#include "Bullet.h"
#include <SerraEngine/InputManager.h>
#include <SerraEngine/Camera2D.h>

#define GUN_EMPTY -1

class Gun;

class Player : public Human
{
	static const SerraEngine::ColorRGBA8 PLAYER_COLOR;
	SerraEngine::InputManager &_inputManager;
	SerraEngine::Camera2D &_camera;
	std::vector<Bullet> &_bullets;
	std::vector<Gun*> _guns;
	int _currentGun;
public:
	explicit Player() = default;
	explicit Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager, SerraEngine::Camera2D &camera, std::vector<Bullet> &bullets);
	~Player();
	void update(float deltaTime = 0.0f,
				const std::vector<std::string>& lvlData = std::vector<std::string>(),
				std::vector<Human*>& humans = std::vector<Human*>()) override;
	void addGun(Gun* gun);
};

