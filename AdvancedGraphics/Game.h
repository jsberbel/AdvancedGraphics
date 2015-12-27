#pragma once
#include <SerraEngine/SerraEngine.h>
#include <SerraEngine/Window.h>
#include <SerraEngine/Sprite.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/FPSLimiter.h>
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/SpriteBatch.h>

enum class GameState{INIT, PLAY, EXIT};

class Game
{
	//attributes
	SerraEngine::GLSLManager _program;
	SerraEngine::Window* _window;
	SerraEngine::Camera2D _camera;
	SerraEngine::SpriteBatch _spriteBatch;
	SerraEngine::FPSLimiter _fpsLimiter;

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
	Game(const std::string &engineName, const int &screenWidth, const int &screenHeight);
	~Game();
	//static GameEngine* Instance(const std::string &engineName, const int &screenWidth, const int &screenHeight);
	void run();
};

