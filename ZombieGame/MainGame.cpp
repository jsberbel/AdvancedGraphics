#include "MainGame.h"
#include <SerraEngine/SerraEngine.h>
#include <SerraEngine/Timing.h>
#include <SerraEngine/ErrorManager.h>
#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include "Zombie.h"
#include "Gun.h"

MainGame::MainGame(const std::string &name, int screenWidth, int screenHeight) :
	_gameState(GameState::PLAY),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_curLevel(0),
	_window(screenWidth, screenHeight, name),
	_camera(screenWidth, screenHeight),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0) {}

MainGame::~MainGame() {
	for (auto l : _levels) if (!l) delete l, l = nullptr;
	for (auto h : _humans) if (!h) delete h, h = nullptr;
	for (auto z : _zombies) if (!z) delete z, z = nullptr;
	if (!_player) delete _player, _player = nullptr;
}

void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	SerraEngine::init();
	_window.createWindow({200, 200, 200, 255});
	//glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	initShaders();
	_agentsBatch.init();
}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_levels.push_back(new Level("Levels/level2.txt"));
	_curLevel = 0;

	_player = new Player(_levels[_curLevel]->getStartPlayerPos(), PLAYER_SPEED, _inputManager, _camera, _bullets);
	_humans.push_back(_player);
	
	std::mt19937 randomEngine;
	randomEngine.seed((unsigned)time(nullptr));
	std::uniform_int_distribution<int> randX(2, (int)_levels[_curLevel]->getWidth()-2);
	std::uniform_int_distribution<int> randY(2, (int)_levels[_curLevel]->getHeight()-2);
	
	//Add humans
	for (int i = 0; i < _levels[_curLevel]->getNumHumans(); i++) {
		glm::vec2 newPos(randX(randomEngine)*Level::TILE_WIDTH, randY(randomEngine)*Level::TILE_WIDTH);
		_humans.push_back(new Human(newPos, HUMAN_SPEED));
	}

	//Add zombies
	const std::list<glm::vec2> &zombiePositions = _levels[_curLevel]->getStartZombiesPos();
	for (auto pos = zombiePositions.begin(), end = zombiePositions.end(); pos != end; ++pos) {
		glm::vec2 newPos(randX(randomEngine)*Level::TILE_WIDTH, randY(randomEngine)*Level::TILE_WIDTH);
		_zombies.push_back(new Zombie(*pos, ZOMBIE_SPEED));
	}
	
	//Add guns
	static const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 5.0f, BULLET_SPEED, 30));
	_player->addGun(new Gun("Shotgun", 30, 12, 20.0f, BULLET_SPEED, 4));
	_player->addGun(new Gun("MP5", 2, 1, 10.0f, BULLET_SPEED, 20));
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::updateAgents(float deltaTime) {
	//Update humans
	for (auto h : _humans) h->update(deltaTime, _levels[_curLevel]->getLevelData());
	/*for (auto i = _humans.begin(), end = _humans.end(); i != end; ++i) {
		for (auto j = std::next(i, 1), end = _humans.end(); j != end; ++j) {
			(*i)->collideWithAgent(*j);
		}
	}*/
	for (unsigned i = 0; i < _humans.size(); i++) {
		for (unsigned j = i+1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

	//Update zombies
	for (auto z : _zombies) z->update(deltaTime, _levels[_curLevel]->getLevelData(), _humans);
	for (unsigned i = 0; i < _zombies.size(); i++) {
		for (unsigned j = i+1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		for (unsigned j = 0; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				if (_humans[j] != _player) {
					_zombies.push_back(new Zombie(_humans[j]->getPosition(), ZOMBIE_SPEED));
					delete _humans[j];
					_humans[j] = _humans.back();
					_humans.pop_back();
				}
				else {
					std::cout << "Zombies ate your brain" << std::endl;
					system("pause");
					_gameState = GameState::EXIT;
				}
			}
		}
	}
	//std::cout << "zombies: " << _zombies.size() << std::endl;
	//std::cout << "humans: " << _humans.size() << std::endl;
}

void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
	for (unsigned i = 0; i < _bullets.size();) {
		if (_bullets[i].update(deltaTime, _levels[_curLevel]->getLevelData())) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else i++;
	}

	bool isBulletRemoved = false;
	// Collide with humans and zombies
	for (unsigned i = 0; i < _bullets.size(); i++) {
		// Check zombies
		for (unsigned j = 0; j < _zombies.size();) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else j++;
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				isBulletRemoved = true;
				i--;
				break;
			}
			else j++;
		}
		// Check humans
		if (!isBulletRemoved) {
			for (unsigned j = 1; j < _humans.size();) {
				if (_bullets[i].collideWithAgent(_humans[j])) {
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) { //if human dies
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else j++;
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					i--;
					break;
				}
				else j++;
			}
		}
	}
}

void MainGame::updateGame() {
	const float DESIRED_FRAMERATE = 1000.0f / DESIRED_FPS;
	const float MAX_DELTATIME = 1.0f;
	const int MAX_PHYSICS_STEPS = 6;

	static float prevTicks = (float)SDL_GetTicks();
	float curTicks = (float)SDL_GetTicks();
	float frameTime = curTicks - prevTicks;
	prevTicks = curTicks;
	float totalDeltaTime = frameTime / DESIRED_FRAMERATE;
	for (int i = 0; totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS; i++) {
		float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);
		updateAgents(deltaTime);
		updateBullets(deltaTime);
		totalDeltaTime -= deltaTime;
	}
	_camera.setPosition(_player->getPosition() + Agent::AGENT_RADIUS);
	_camera.update();
}

void MainGame::gameLoop() {
	SerraEngine::FPSLimiter fpsLimiter(DESIRED_FPS);

	while (_gameState != GameState::EXIT) {
		fpsLimiter.begin();
			checkEndGame();
			processInput();
			updateGame();
			drawGame();
			fpsLimiter.getFPS();
		fpsLimiter.end();
	}
}

void MainGame::processInput() {
    SDL_Event evnt;
	bool zoomOut = false, zoomIn = false;

	_inputManager.update();

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
				_gameState = GameState::EXIT;
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
			case SDL_MOUSEWHEEL:
				if (evnt.wheel.y > 0) zoomIn = true;
				else zoomOut = true;
				break;
        }
    }

	if (_inputManager.isKeyDown(SDLK_ESCAPE)) _gameState = GameState::EXIT;

	static float camScale = _camera.getScale();
	if (camScale > 0.1f && zoomOut) camScale -= 0.02f;
	if (camScale < 2.0f && zoomIn) camScale += 0.02f;
	_camera.setScale(camScale);
	
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

	//Draw current level
	_levels[_curLevel]->draw();

	_agentsBatch.begin();

	//Draw the agents
	for (auto h : _humans) h->pushBatch(_agentsBatch);
	for (auto z : _zombies) z->pushBatch(_agentsBatch);

	//Draw bullets
	for (auto &b : _bullets) b.pushBatch(_agentsBatch);

	_agentsBatch.end();
	_agentsBatch.renderBatch();
   
	_textureProgram.unbind();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}

void MainGame::checkEndGame() {
	// Support for multiple levels:
	// _curLevel++; initLevel()...

	if (_zombies.empty()) {
		std::printf("*** YOU WIN! ***\nYou killed %d humans and %d zombies. You saved %d/%d civilians remaining.\n", _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_curLevel]->getNumHumans());
		system("pause");
		_gameState = GameState::EXIT;
	}
}