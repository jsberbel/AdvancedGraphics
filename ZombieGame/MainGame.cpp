#include "MainGame.h"
#include <SerraEngine/SerraEngine.h>
#include <SerraEngine/Timing.h>
#include <SerraEngine/ResourceManager.h>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <SDL/SDL.h>
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
	m_bloodParticleBatch(nullptr),
	m_player(nullptr) {}

MainGame::~MainGame() {
	for (auto l : m_levels) if (l != nullptr) delete l, l = nullptr;
	for (auto h : m_humans) if (h != nullptr) delete h, h = nullptr;
	for (auto z : m_zombies) if (z != nullptr) delete z, z = nullptr;
}

void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	SerraEngine::init();
	m_window.createWindow(SerraEngine::ColorRGBA8{125, 125, 125, 255});
	m_audioManager.init();
	initShaders();
	m_mainSpriteBatch.init();
	m_HUDBatch.init();
	m_HUDcamera.setPosition(glm::vec2(m_screenWidth*0.5f, m_screenHeight*0.5f));
	m_spriteFont = std::make_unique<SerraEngine::SpriteFont>("Fonts/ComickBook_Simple.ttf", 64);

	//m_bloodParticleBatch = std::make_unique<SerraEngine::ParticleBatch2D>();
	m_bloodParticleBatch = new SerraEngine::ParticleBatch2D();
	m_bloodParticleBatch->init(1000, 0.05f, SerraEngine::ResourceManager::getTexture("Textures/particle.png").id);
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);

	m_curMusic = m_audioManager.loadMusic("Sound/XYZ.ogg");
	m_curMusic.play();
}

void MainGame::initLevel() {
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_levels.push_back(new Level("Levels/level2.txt"));
	m_curLevel = 0;

	m_player = new Player(m_levels[m_curLevel]->getStartPlayerPos(), PLAYER_SPEED, m_inputManager, m_camera, m_bullets);
	m_humans.push_back(m_player);
	
	m_randomEngine.seed(static_cast<unsigned>(time(nullptr)));
	std::uniform_int_distribution<int> randX(2, static_cast<int>(m_levels[m_curLevel]->getWidth())-2);
	std::uniform_int_distribution<int> randY(2, static_cast<int>(m_levels[m_curLevel]->getHeight())-2);
	
	//Add humans
	for (auto i = 0, end = m_levels[m_curLevel]->getNumHumans(); i < end; i++) {
		glm::vec2 newPos(randX(m_randomEngine)*Level::TILE_WIDTH, randY(m_randomEngine)*Level::TILE_WIDTH);
		m_humans.push_back(new Human(newPos, HUMAN_SPEED));
	}

	//Add zombies
	const auto &zombiePositions = m_levels[m_curLevel]->getStartZombiesPos();
	for (auto pos = zombiePositions.begin(), end = zombiePositions.end(); pos != end; ++pos) {
		m_zombies.push_back(new Zombie(*pos, ZOMBIE_SPEED));
	}
	
	//Add guns
	const auto BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 5.0f, BULLET_SPEED, 30, m_audioManager.loadSoundEffect(("Sound/shots/pistol.wav"))));
	m_player->addGun(new Gun("Shotgun", 30, 12, 20.0f, BULLET_SPEED, 10, m_audioManager.loadSoundEffect(("Sound/shots/shotgun.wav"))));
	m_player->addGun(new Gun("MP5", 2, 1, 10.0f, BULLET_SPEED, 20, m_audioManager.loadSoundEffect(("Sound/shots/cg1.wav"))));
}

void MainGame::initShaders() {
    //Compile our color shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	SerraEngine::FPSLimiter fpsLimiter(DESIRED_FPS);

	while (m_gameState != GameState::EXIT) {
		switch (m_gameState) {
		case GameState::PLAY:
			fpsLimiter.begin();
				checkEndGame();
				processInput();
				updateGame();
				drawGame();
				fpsLimiter.getFPS();
			fpsLimiter.end();
			break;
		case GameState::GAMEOVER:
			gameOver();
			break;
		default: break;
		}
	}
}

void MainGame::updateAgents(float deltaTime) {
	/*--------------------
		UPDATE HUMANS
	--------------------*/
	//Update rand position
	for (const auto h : m_humans) h->update(deltaTime, m_levels[m_curLevel]->getLevelData());
	//Check collisions between other humans
	for (unsigned i = 0; i < m_humans.size(); i++) {
		for (auto j = i+1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}

	/*--------------------
		UPDATE ZOMBIES
	--------------------*/
	//Update position seeking humans
	for (const auto z : m_zombies) z->update(deltaTime, m_levels[m_curLevel]->getLevelData(), m_humans);
	//Check collisions between other zombies and humans
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
				else m_playerDead = true, m_gameState = GameState::GAMEOVER;
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

	auto isBulletRemoved = false;
	// Collide with humans and zombies
	for (unsigned i = 0; i < m_bullets.size(); i++) {
		// Check zombies
		for (unsigned j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				addBlood(m_bullets[i].getPosition(), 5); //blood particle system
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
					addBlood(m_bullets[i].getPosition(), 5); //blood particle system
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
		m_particleEngine.update(deltaTime);
		totalDeltaTime -= deltaTime;
	}
	m_camera.setPosition(m_player->getPosition() + Agent::AGENT_RADIUS);
	m_camera.update();
	m_HUDcamera.update();
}

void MainGame::gameOver() {
	m_inputManager.update();
	if (m_inputManager.isKeyDown(SDLK_ESCAPE)) m_gameState = GameState::EXIT;

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) if (evnt.type == SDL_QUIT) m_gameState = GameState::EXIT;

	if (m_curMusic.isPlaying()) m_curMusic.stop();
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

	m_mainSpriteBatch.begin();

	//Draw the agents
	const glm::vec2 agentDims(Agent::AGENT_RADIUS*2.0f);
	for (const auto h : m_humans) if (m_camera.isBoxInView(h->getPosition(), agentDims)) h->pushBatch(m_mainSpriteBatch);
	for (const auto z : m_zombies) if (m_camera.isBoxInView(z->getPosition(), agentDims)) z->pushBatch(m_mainSpriteBatch);

	//Draw bullets
	for (auto &b : m_bullets) b.pushBatch(m_mainSpriteBatch);

	m_mainSpriteBatch.end();
	m_mainSpriteBatch.renderBatch();

	m_particleEngine.draw(m_mainSpriteBatch);

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
		if (m_playerDead) {
			strcpy_s(buffer, "ZOMBIES ATE YOUR BRAIN");
			m_spriteFont->draw(m_HUDBatch, buffer, glm::vec2(m_screenWidth*0.5f, m_screenHeight*0.5f), glm::vec2(0.7f), 0.0f,
				SerraEngine::ColorRGBA8(150, 0, 0, 255), SerraEngine::Justification::MIDDLE);
		}
		sprintf_s(buffer, "Num humans: %d", m_humans.size()-1);
		m_spriteFont->draw(m_HUDBatch, buffer, glm::vec2(50, 50), glm::vec2(0.5f), 0.0f,
			SerraEngine::ColorRGBA8(255,255,255,255));
	m_HUDBatch.end();
	m_HUDBatch.renderBatch();
}

void MainGame::checkEndGame() {
	// Support for multiple levels:
	// _curLevel++; initLevel()...

	if (m_zombies.empty()) {
		printf("*** YOU WIN! ***\nYou killed %d humans and %d zombies. You saved %d/%d civilians remaining.\n", 
			m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_curLevel]->getNumHumans());
		system("pause");
		m_gameState = GameState::EXIT;
	}
}

void MainGame::addBlood(const glm::vec2 & position, int numParticles) {
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 vel(2.0f, 0.0f);
	SerraEngine::ColorRGBA8 col(200, 0, 0, 255);
	for (auto i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(m_randomEngine)), 20.0f, col);
	}
}
