#pragma once
#include <SerraEngine/Window.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/InputManager.h>
#include "Level.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
	std::vector<Level*> _levels; //List of all levels
	GameState _gameState;
	int _screenWidth, _screenHeight;
	int _curLevel;

	SerraEngine::GLWindow _window; // The game window
	SerraEngine::GLSLManager _textureProgram; // The shader program
	SerraEngine::InputManager _inputManager; // Handles input
	SerraEngine::Camera2D _camera; // Main Camera

	
	void initSystems();
	void initLevel();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
public:
    MainGame(const std::string &name, int screenWidth, int screenHeight);
    ~MainGame();
   
    void run();
};

