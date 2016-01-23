#include "Player.h"
#include "Gun.h"
#include <SDL/SDL.h>

const SerraEngine::ColorRGBA8 Player::PLAYER_COLOR = SerraEngine::ColorRGBA8{ 0,0, 185, 255 };

Player::Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager, SerraEngine::Camera2D &camera, std::vector<Bullet> &bullets) :
	Human(position, speed, PLAYER_COLOR),
	m_inputManager(inputManager),
	m_camera(camera),
	m_bullets(bullets),
	m_currentGun(GUN_EMPTY) {
	_health = 150;
}

Player::~Player() {
	for (auto g : m_guns) if (!g) delete g, g = nullptr;
}

void Player::update(float deltaTime, const std::vector<std::string>& lvlData, const std::vector<Human*>& humans) {
	static auto dirCol = false;

	if (m_inputManager.isKeyDown(SDLK_w))	  _position.y += _speed*deltaTime, dirCol = true;
	else if (m_inputManager.isKeyDown(SDLK_s)) _position.y -= _speed*deltaTime, dirCol = false;

	if (m_inputManager.isKeyDown(SDLK_a))	  _position.x -= _speed*deltaTime;
	else if (m_inputManager.isKeyDown(SDLK_d)) _position.x += _speed*deltaTime;

	if (m_inputManager.isKeyDown(SDLK_1) && m_guns.size() >= 0) m_currentGun = 0;
	if (m_inputManager.isKeyDown(SDLK_2) && m_guns.size() >= 1) m_currentGun = 1;
	if (m_inputManager.isKeyDown(SDLK_3) && m_guns.size() >= 2) m_currentGun = 2;

	if (m_currentGun != GUN_EMPTY) {
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);
		m_guns[m_currentGun]->update(m_inputManager.isKeyDown(SDL_BUTTON_LEFT),
								   centerPosition,
								   direction, 
								   m_bullets,
								   deltaTime);
	}

	collideWithLevel(lvlData, dirCol);
}

void Player::addGun(Gun * gun) {
	m_guns.push_back(gun);
	if (m_currentGun == GUN_EMPTY) {
		m_currentGun = 0;
	}
}
