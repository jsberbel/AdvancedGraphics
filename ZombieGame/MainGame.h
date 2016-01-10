#pragma once
#include <SerraEngine/Window.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/InputManager.h>
#include <SerraEngine/SpriteBatch.h>
#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

enum class GameState { PLAY, EXIT };

class MainGame
{
	static const float PLAYER_SPEED;
	static const float HUMAN_SPEED;
	static const float ZOMBIE_SPEED;

	std::vector<Level*> _levels; //List of all levels
	GameState _gameState;
	int _screenWidth, _screenHeight;
	int _curLevel;

	SerraEngine::GLWindow _window; // The game window
	SerraEngine::GLSLManager _textureProgram; // The shader program
	SerraEngine::InputManager _inputManager; // Handles input
	SerraEngine::Camera2D _camera; // Main Camera
	SerraEngine::SpriteBatch _agentsBatch;

	Player* _player;
	std::vector<Human*> _humans; //vector of all humans
	std::vector<Zombie*> _zombies; //vector of all zombies
	std::vector<Bullet> _bullets; //vector of all bullets

	int _numHumansKilled;
	int _numZombiesKilled;
	
	void initSystems();
	void initLevel();
	void initShaders();
	void updateAgents();
	void updateBullets();
	void updateGame();
	void gameLoop();
	void processInput();
	void drawGame();
	void checkEndGame();
public:
    MainGame(const std::string &name, int screenWidth, int screenHeight);
    ~MainGame();
   
    void run();
};

