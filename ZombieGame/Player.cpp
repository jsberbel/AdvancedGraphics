#include "Player.h"
#include <SDL/SDL.h>

Player::Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager) :
	_inputManager(inputManager),
	Human(position, speed, PLAYER_COLOR) {}

void Player::update(std::vector<Human*> &humans, std::vector<Zombie*> &zombies) {
	static bool dirCol = false;

	if (_inputManager.isKeyPressed(SDLK_w))		 _position.y += _speed, dirCol = true;
	else if (_inputManager.isKeyPressed(SDLK_s)) _position.y -= _speed, dirCol = false;

	if (_inputManager.isKeyPressed(SDLK_a))		 _position.x -= _speed;
	else if (_inputManager.isKeyPressed(SDLK_d)) _position.x += _speed;

	collideWithLevel(dirCol);
}
