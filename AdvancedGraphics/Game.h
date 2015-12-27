#pragma once
#include <iostream>
#include <SerraEngine/Window.h>
#include <SerraEngine/Sprite.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/FPSLimiter.h>

#define FULL_SCREEN false

enum class GameState{INIT, PLAY, EXIT};

class Game
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
	Game(const std::string &engineName, const int &screenWidth, const int &screenHeight);
	~Game();
	//static GameEngine* Instance(const std::string &engineName, const int &screenWidth, const int &screenHeight);
	void run();
};

