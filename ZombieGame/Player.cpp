#include "Player.h"
#include "Gun.h"
#include <SDL/SDL.h>
#include <SerraEngine/ResourceManager.h>


Player::Player(const glm::vec2 & position, float speed, SerraEngine::InputManager &inputManager, SerraEngine::Camera2D &camera, std::vector<Bullet> &bullets, const SerraEngine::ColorRGBA8 &color) :
	Human(position, speed, color),
	m_inputManager(inputManager),
	m_camera(camera),
	m_bullets(bullets),
	m_currentGun(GUN_EMPTY) {
	m_health = 150;
	m_textureID = SerraEngine::ResourceManager::getTexture("Textures/player.png").id;
}

Player::~Player() {
	for (auto g : m_guns) if (!g) delete g, g = nullptr;
}

void Player::update(float deltaTime, const std::vector<std::string>& lvlData, const std::vector<Human*>& humans) {
	static auto dirCol = false;

	if (m_inputManager.isKeyDown(SDLK_w))	  m_position.y += m_speed*deltaTime, dirCol = true;
	else if (m_inputManager.isKeyDown(SDLK_s)) m_position.y -= m_speed*deltaTime, dirCol = false;

	if (m_inputManager.isKeyDown(SDLK_a))	  m_position.x -= m_speed*deltaTime;
	else if (m_inputManager.isKeyDown(SDLK_d)) m_position.x += m_speed*deltaTime;

	if (m_inputManager.isKeyDown(SDLK_1) && m_guns.size() >= 0) m_currentGun = 0;
	if (m_inputManager.isKeyDown(SDLK_2) && m_guns.size() >= 1) m_currentGun = 1;
	if (m_inputManager.isKeyDown(SDLK_3) && m_guns.size() >= 2) m_currentGun = 2;

	auto mouseCoords{ m_camera.convertScreenToWorld(m_inputManager.getMouseCoords()) };

	auto centerPosition{ m_position + glm::vec2(AGENT_RADIUS) };
	m_direction = glm::normalize(mouseCoords - centerPosition);

	if (m_currentGun != GUN_EMPTY) {
		m_guns[m_currentGun]->update(m_inputManager.isKeyDown(SDL_BUTTON_LEFT),
									centerPosition,
									m_direction,
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
