#include "Player.h"
#include <SDL/SDL.h>

Player::Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager) :
	_inputManager(inputManager),
	Human(position, speed, PLAYER_COLOR) {}

Player::~Player()
{
}

void Player::init(const glm::vec2 &position, float speed, SerraEngine::InputManager &inputManager) {
	 _inputManager = inputManager;
	_position = position;
	_speed = speed;
	_color = PLAYER_COLOR;
}

void Player::update() {
	if (_inputManager.isKeyPressed(SDLK_w))		_position.y += _speed;
	else if (_inputManager.isKeyPressed(SDLK_s)) _position.y -= _speed;

	if (_inputManager.isKeyPressed(SDLK_a))		_position.x -= _speed;
	else if (_inputManager.isKeyPressed(SDLK_d)) _position.x += _speed;
}
