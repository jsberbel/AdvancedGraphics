#pragma once
#include <SerraEngine/Window.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/InputManager.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/SpriteFont.h>
#include <SerraEngine/AudioManager.h>
#include "Player.h"
#include "Level.h"
#include "Bullet.h"
#include <memory>

class Zombie;

enum class GameState { PLAY, EXIT };

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
	SerraEngine::SpriteBatch m_agentsBatch;
	SerraEngine::SpriteBatch m_HUDBatch;
	std::unique_ptr<SerraEngine::SpriteFont> m_spriteFont;
	SerraEngine::AudioManager m_audioManager;

	Player* m_player;
	std::vector<Human*> m_humans; //vector of all humans
	std::vector<Zombie*> m_zombies; //vector of all zombies
	std::vector<Bullet> m_bullets; //vector of all bullets

	int m_numHumansKilled = 0;
	int m_numZombiesKilled = 0;
	
	void initSystems();
	void initLevel();
	void initShaders();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void updateGame();
	void gameLoop();
	void processInput();
	void drawGame();
	void drawHUD();
	void checkEndGame();
public:
    MainGame(const std::string &name, int screenWidth, int screenHeight);
    ~MainGame();
   
    void run();
};

