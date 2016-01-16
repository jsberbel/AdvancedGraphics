#include "Player.h"
#include "Gun.h"
#include <SDL/SDL.h>

Player::Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager, SerraEngine::Camera2D &camera, std::vector<Bullet> &bullets) :
	_inputManager(inputManager),
	_camera(camera),
	_bullets(bullets),
	_currentGun(GUN_EMPTY),
	Human(position, speed, PLAYER_COLOR) {
	_health = 150;
}

Player::~Player() {
	for (auto g : _guns) if (!g) delete g, g = nullptr;
}

void Player::update(float deltaTime, const std::vector<std::string>& lvlData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	static bool dirCol = false;

	if (_inputManager.isKeyDown(SDLK_w))	  _position.y += _speed*deltaTime, dirCol = true;
	else if (_inputManager.isKeyDown(SDLK_s)) _position.y -= _speed*deltaTime, dirCol = false;

	if (_inputManager.isKeyDown(SDLK_a))	  _position.x -= _speed*deltaTime;
	else if (_inputManager.isKeyDown(SDLK_d)) _position.x += _speed*deltaTime;

	if (_inputManager.isKeyDown(SDLK_1) && _guns.size() >= 0) _currentGun = 0;
	if (_inputManager.isKeyDown(SDLK_2) && _guns.size() >= 1) _currentGun = 1;
	if (_inputManager.isKeyDown(SDLK_3) && _guns.size() >= 2) _currentGun = 2;

	if (_currentGun != GUN_EMPTY) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);
		_guns[_currentGun]->update(_inputManager.isKeyDown(SDL_BUTTON_LEFT),
								   centerPosition,
								   direction, 
								   _bullets,
								   deltaTime);
	}

	collideWithLevel(lvlData, dirCol);
}

void Player::addGun(Gun * gun) {
	_guns.push_back(gun);
	if (_currentGun == GUN_EMPTY) {
		_currentGun = 0;
	}
}
