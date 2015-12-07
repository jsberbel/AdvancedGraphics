#pragma once
#include <thread> 
#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "Window.h"
#include "Sprite.h"
#include "GLSLManager.h"
#include "GLTexture.h"

#define FULL_SCREEN false

enum class GameState{INIT, PLAY, EXIT};

class GameEngine
{
	//attributes
	GLSLManager _program;
	Window* _window;
	Sprite _sprite;
	GLTexture _texture;
	GameState _gameState;
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

