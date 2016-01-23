#pragma once
#include <SerraEngine/Window.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/InputManager.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/SpriteFont.h>
#include <SerraEngine/AudioManager.h>
#include <SerraEngine/ParticleEngine2D.h>
#include <SerraEngine/ParticleBatch2D.h>
#include "Player.h"
#include "Level.h"
#include "Bullet.h"
#include <memory>
#include <random>

class Zombie;

enum class GameState { PLAY, GAMEOVER, EXIT };

class MainGame
{
	const float DESIRED_FPS = 120.0f;
	const float PLAYER_SPEED = 4.0f;
	const float HUMAN_SPEED = 1.5f;
	const float ZOMBIE_SPEED = 1.8f;

	std::vector<Level*> m_levels; //List of all levels
	int m_screenWidth, m_screenHeight;
	GameState m_gameState = GameState::PLAY;
	int m_curLevel = 0;

	SerraEngine::GLWindow m_window; // The game window
	SerraEngine::GLSLManager m_textureProgram; // The shader program
	SerraEngine::InputManager m_inputManager; // Handles input
	SerraEngine::Camera2D m_camera; // Main Camera
	SerraEngine::Camera2D m_HUDcamera;
	SerraEngine::SpriteBatch m_mainSpriteBatch;
	SerraEngine::SpriteBatch m_HUDBatch;
	std::unique_ptr<SerraEngine::SpriteFont> m_spriteFont;
	SerraEngine::AudioManager m_audioManager;

	SerraEngine::ParticleEngine2D m_particleEngine;
	SerraEngine::ParticleBatch2D* m_bloodParticleBatch;

	Player* m_player;
	std::vector<Human*> m_humans; //vector of all humans
	std::vector<Zombie*> m_zombies; //vector of all zombies
	std::vector<Bullet> m_bullets; //vector of all bullets

	int m_numHumansKilled = 0;
	int m_numZombiesKilled = 0;
	bool m_playerDead = false;
	SerraEngine::Music m_curMusic;
	std::mt19937 m_randomEngine;
	
	void initSystems();
	void initLevel();
	void initShaders();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void updateGame();
	void gameOver();
	void processInput();
	void drawGame();
	void drawHUD();
	void checkEndGame();
	void addBlood(const glm::vec2 &position, int numParticles);
public:
    MainGame(const std::string &name, int screenWidth, int screenHeight);
    ~MainGame();
   
    void run();
};

