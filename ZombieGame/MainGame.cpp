#include "MainGame.h"
#include <SerraEngine/SerraEngine.h>
#include <SerraEngine/Timing.h>
#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include "Zombie.h"
#include "Gun.h"

MainGame::MainGame(const std::string &name, int screenWidth, int m_screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(m_screenHeight),
	m_window(screenWidth, m_screenHeight, name),
	m_camera(screenWidth, m_screenHeight),
	m_HUDcamera(screenWidth, m_screenHeight),
	m_spriteFont(nullptr),
	m_player(nullptr) {}

MainGame::~MainGame() {
	for (auto l : m_levels) if (!l) delete l, l = nullptr;
	for (auto h : m_humans) if (!h) delete h, h = nullptr;
	for (auto z : m_zombies) if (!z) delete z, z = nullptr;
	if (!m_player) delete m_player, m_player = nullptr;
}

void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	SerraEngine::init();
	m_window.createWindow(SerraEngine::ColorRGBA8{200, 200, 200, 255});
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	initShaders();
	m_agentsBatch.init();
	m_HUDBatch.init();
	m_HUDcamera.setPosition(glm::vec2(m_screenWidth*0.5f, m_screenHeight*0.5f));
	m_spriteFont = std::make_unique<SerraEngine::SpriteFont>("Fonts/ComickBook_Simple.ttf", 64);
}

void MainGame::initLevel() {
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_levels.push_back(new Level("Levels/level2.txt"));
	m_curLevel = 0;

	m_player = new Player(m_levels[m_curLevel]->getStartPlayerPos(), PLAYER_SPEED, m_inputManager, m_camera, m_bullets);
	m_humans.push_back(m_player);
	
	std::mt19937 randomEngine;
	randomEngine.seed(static_cast<unsigned>(time(nullptr)));
	std::uniform_int_distribution<int> randX(2, static_cast<int>(m_levels[m_curLevel]->getWidth())-2);
	std::uniform_int_distribution<int> randY(2, static_cast<int>(m_levels[m_curLevel]->getHeight())-2);
	
	//Add humans
	for (auto i = 0, end = m_levels[m_curLevel]->getNumHumans(); i < end; i++) {
		glm::vec2 newPos(randX(randomEngine)*Level::TILE_WIDTH, randY(randomEngine)*Level::TILE_WIDTH);
		m_humans.emplace_back(new Human(newPos, HUMAN_SPEED));
	}

	//Add zombies
	const auto &zombiePositions = m_levels[m_curLevel]->getStartZombiesPos();
	for (auto pos = zombiePositions.begin(), end = zombiePositions.end(); pos != end; ++pos) {
		glm::vec2 newPos(randX(randomEngine)*Level::TILE_WIDTH, randY(randomEngine)*Level::TILE_WIDTH);
		m_zombies.push_back(new Zombie(newPos, ZOMBIE_SPEED));
	}
	
	//Add guns
	const auto BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 5.0f, BULLET_SPEED, 30));
	m_player->addGun(new Gun("Shotgun", 30, 12, 20.0f, BULLET_SPEED, 10));
	m_player->addGun(new Gun("MP5", 2, 1, 10.0f, BULLET_SPEED, 20));
}

void MainGame::initShaders() {
    // Compile our color shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

void MainGame::updateAgents(float deltaTime) {
	//Update humans
	for (const auto h : m_humans) h->update(deltaTime, m_levels[m_curLevel]->getLevelData());
	/*for (auto i = _humans.begin(), end = _humans.end(); i != end; ++i) {
		for (auto j = std::next(i, 1), end = _humans.end(); j != end; ++j) {
			(*i)->collideWithAgent(*j);
		}
	}*/
	for (unsigned i = 0; i < m_humans.size(); i++) {
		for (auto j = i+1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}

	//Update zombies
	for (const auto z : m_zombies) z->update(deltaTime, m_levels[m_curLevel]->getLevelData(), m_humans);
	for (unsigned i = 0; i < m_zombies.size(); i++) {
		for (auto j = i+1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		for (unsigned j = 0; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				if (m_humans[j] != m_player) {
					m_zombies.push_back(new Zombie(m_humans[j]->getPosition(), ZOMBIE_SPEED));
					delete m_humans[j];
					m_humans[j] = m_humans.back();
					m_humans.pop_back();
				}
				else {
					std::cout << "Zombies ate your brain" << std::endl;
					system("pause");
					m_gameState = GameState::EXIT;
				}
			}
		}
	}
	//std::cout << "zombies: " << _zombies.size() << std::endl;
	//std::cout << "humans: " << _humans.size() << std::endl;
}

void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
	for (unsigned i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(deltaTime, m_levels[m_curLevel]->getLevelData())) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else i++;
	}

	bool isBulletRemoved = false;
	// Collide with humans and zombies
	for (unsigned i = 0; i < m_bullets.size(); i++) {
		// Check zombies
		for (unsigned j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) { //if zombie dies
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				}
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				isBulletRemoved = true;
				i--;
				break;
			}
			j++;
		}
		// Check humans
		if (!isBulletRemoved) {
			for (unsigned j = 1; j < m_humans.size();) {
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) { //if human dies
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_numHumansKilled++;
					}
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					i--;
					break;
				}
				j++;
			}
		}
	}
}

void MainGame::updateGame() {
	const auto DESIRED_FRAMERATE = 1000.0f / DESIRED_FPS;
	const auto MAX_DELTATIME = 1.0f;
	const auto MAX_PHYSICS_STEPS = 6;

	static auto prevTicks = static_cast<float>(SDL_GetTicks());
	auto curTicks = static_cast<float>(SDL_GetTicks());
	auto frameTime = curTicks - prevTicks;
	prevTicks = curTicks;
	auto totalDeltaTime = frameTime / DESIRED_FRAMERATE;
	for (auto i = 0; totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS; i++) {
		auto deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);
		updateAgents(deltaTime);
		updateBullets(deltaTime);
		totalDeltaTime -= deltaTime;
	}
	m_camera.setPosition(m_player->getPosition() + Agent::AGENT_RADIUS);
	m_camera.update();
	m_HUDcamera.update();
}

void MainGame::gameLoop() {
	SerraEngine::FPSLimiter fpsLimiter(DESIRED_FPS);

	while (m_gameState != GameState::EXIT) {
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
	auto zoomOut = false, zoomIn = false;

	m_inputManager.update();

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
				m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y));
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
			case SDL_MOUSEWHEEL:
				if (evnt.wheel.y > 0) zoomIn = true;
				else zoomOut = true;
				break;
        }
    }

	if (m_inputManager.isKeyDown(SDLK_ESCAPE)) m_gameState = GameState::EXIT;

	static auto camScale = m_camera.getScale()*0.5f;
	if (camScale > 0.1f && zoomOut) camScale -= 0.02f;
	if (camScale < 2.0f && zoomIn) camScale += 0.02f;
	m_camera.setScale(camScale);
	
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.bind();

	glActiveTexture(GL_TEXTURE0);
	auto textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	auto projectionUniform = m_textureProgram.getUniformLocation("P");
	auto projectionMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw current level
	m_levels[m_curLevel]->draw();

	m_agentsBatch.begin();

	//Draw the agents
	const glm::vec2 agentDims(Agent::AGENT_RADIUS*2.0f);
	for (const auto h : m_humans) if (m_camera.isBoxInView(h->getPosition(), agentDims)) h->pushBatch(m_agentsBatch);
	for (const auto z : m_zombies) if (m_camera.isBoxInView(z->getPosition(), agentDims)) z->pushBatch(m_agentsBatch);

	//Draw bullets
	for (auto &b : m_bullets) b.pushBatch(m_agentsBatch);

	m_agentsBatch.end();
	m_agentsBatch.renderBatch();

	drawHUD();
   
	m_textureProgram.unbind();

    // Swap our buffer and draw everything to the screen!
    m_window.swapBuffer();
}

void MainGame::drawHUD() {
	char buffer[256];
	auto projectionUniform = m_textureProgram.getUniformLocation("P");
	auto projectionMatrix = m_HUDcamera.getCameraMatrix();
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
	m_HUDBatch.begin();
		sprintf_s(buffer, "Num humans: %d", m_humans.size());
		m_spriteFont->draw(m_HUDBatch, buffer, glm::vec2(50, 50), glm::vec2(0.5f), 0.0f,
			SerraEngine::ColorRGBA8(255,255,255,255));
	m_HUDBatch.end();
	m_HUDBatch.renderBatch();
}

void MainGame::checkEndGame() {
	// Support for multiple levels:
	// _curLevel++; initLevel()...

	if (m_zombies.empty()) {
		std::printf("*** YOU WIN! ***\nYou killed %d humans and %d zombies. You saved %d/%d civilians remaining.\n", m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_curLevel]->getNumHumans());
		system("pause");
		m_gameState = GameState::EXIT;
	}
}