#include "MainGame.h"
#include <SerraEngine/SerraEngine.h>
#include <SerraEngine/Timing.h>
#include <SDL/SDL.h>
#include <iostream>

MainGame::MainGame(const std::string &name, int screenWidth, int screenHeight) :
	_gameState(GameState::PLAY),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_curLevel(0),
	_window(screenWidth, screenHeight, name),
	_camera(screenWidth, screenHeight) {}

MainGame::~MainGame() {
	for (auto l : _levels) delete l, l = nullptr;
}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	SerraEngine::init();
	_window.createWindow({200, 200, 200, 255});
	//glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	initShaders();
	initLevel();
}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_curLevel = 0;
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	SerraEngine::FPSLimiter fpsLimiter(120.0f);

	while (_gameState != GameState::EXIT) {
		fpsLimiter.begin();
		processInput();
		_camera.update();
		drawGame();
		fpsLimiter.end();
	}
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.bind();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint projectionUniform = _textureProgram.getUniformLocation("P");
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_levels[_curLevel]->draw();
   
	_textureProgram.unbind();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}