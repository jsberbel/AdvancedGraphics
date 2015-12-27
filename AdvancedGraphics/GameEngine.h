#pragma once
#include <iostream>
#include "Window.h"
#include "Sprite.h"
#include "GLSLManager.h"
#include "FPSLimiter.h"

#define FULL_SCREEN false

enum class GameState{INIT, PLAY, EXIT};

class GameEngine
{
	//attributes
	GLSLManager _program;
	Window* _window;
	Sprite _sprite;
	GameState _gameState;
	FPSLimiter _fpsLimiter;
	int _screenWidth;
	int _screenHeight;

	float _time;

	//private methods
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	
public:
	GameEngine(const std::string &engineName, const int &screenWidth, const int &screenHeight);
	~GameEngine();
	//static GameEngine* Instance(const std::string &engineName, const int &screenWidth, const int &screenHeight);
	void run();
};

