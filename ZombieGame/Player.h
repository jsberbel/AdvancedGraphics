#pragma once
#include "Human.h"
#include <SerraEngine/InputManager.h>

#define PLAYER_COLOR { 0,0, 185, 255 }

class Player : public Human
{
	SerraEngine::InputManager &_inputManager;
public:
	Player() = default;
	Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager);
	~Player();
	void init(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager);
	void update();
};

