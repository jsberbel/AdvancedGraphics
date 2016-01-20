#pragma once
#include <SerraEngine/Window.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/InputManager.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/SpriteFont.h>
#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

enum class GameState { PLAY, EXIT };

class MainGame
{
	const float DESIRED_FPS = 120.0f;
	const float PLAYER_SPEED = 4.0f;
	const float HUMAN_SPEED = 1.5f;
	const float ZOMBIE_SPEED = 1.8f;

	std::vector<Level*> _levels; //List of all levels
	int _screenWidth, m_screenHeight;
	GameState _gameState = GameState::PLAY;
	int _curLevel = 0;

	SerraEngine::GLWindow _window; // The game window
	SerraEngine::GLSLManager _textureProgram; // The shader program
	SerraEngine::InputManager _inputManager; // Handles input
	SerraEngine::Camera2D _camera; // Main Camera
	SerraEngine::Camera2D _HUDcamera;
	SerraEngine::SpriteBatch _agentsBatch;
	SerraEngine::SpriteBatch _HUDBatch;
	std::unique_ptr<SerraEngine::SpriteFont> _spriteFont;

	Player* _player;
	std::vector<Human*> _humans; //vector of all humans
	std::vector<Zombie*> _zombies; //vector of all zombies
	std::vector<Bullet> _bullets; //vector of all bullets

	int _numHumansKilled = 0;
	int _numZombiesKilled = 0;
	
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

