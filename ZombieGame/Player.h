#pragma once
#include "Human.h"
#include "Bullet.h"
#include <SerraEngine/InputManager.h>
#include <SerraEngine/Camera2D.h>

#define GUN_EMPTY -1

class Gun;

class Player : public Human
{
	SerraEngine::InputManager &m_inputManager;
	SerraEngine::Camera2D &m_camera;
	std::vector<Bullet> &m_bullets;
	std::vector<Gun*> m_guns;
	int m_currentGun;
public:
	explicit Player() = default;
	explicit Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager, SerraEngine::Camera2D &camera, std::vector<Bullet> &bullets, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 255, 255, 255, 255 });
	~Player();
	void update(float deltaTime = 0.0f,
				const std::vector<std::string>& lvlData = std::vector<std::string>(),
				const std::vector<Human*>& humans = std::vector<Human*>()) override;
	void addGun(Gun* gun);
};

