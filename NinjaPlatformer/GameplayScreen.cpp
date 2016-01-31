#include "GameplayScreen.h"
#include <SDL/SDL.h>
#include <SerraEngine/IMainGame.h>
#include <random>
#include <ctime>
#include "Light.h"
#include <iostream>

void GameplayScreen::checkInput() const {
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}

GameplayScreen::GameplayScreen(SerraEngine::GLWindow* window) :
	m_window(window) {}

GameplayScreen::~GameplayScreen() {
}

void GameplayScreen::build() {
	
}

void GameplayScreen::destroy() {
	
}

void GameplayScreen::onEntry() {
	b2Vec2 gravity(0.0f, -20.0f);
	m_world = std::make_unique<b2World>(gravity);

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Make bunch of boxes
	std::mt19937 randGenerator(static_cast<unsigned>(time(nullptr)));
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> col(50, 255);
	const int NUM_BOXES = 10;
	for (int i = 0; i < NUM_BOXES; i++) m_boxes.emplace_back(m_world.get(), 
															 glm::vec2(xPos(randGenerator), yPos(randGenerator)), 
															 glm::vec2(size(randGenerator), size(randGenerator)), 
															 "Textures/bricks_top.png",
															 SerraEngine::ColorRGBA8(col(randGenerator), col(randGenerator), col(randGenerator), 255)); //Create new box
	
	m_spriteBatch.init(); //Initialize spritebatch
	m_debugRenderer.init();

	//Initialize texture shaders
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
	//Initialize light shaders
	m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f);

	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(2.0f), glm::vec2(1.0f, 1.8f), "Textures/blue_ninja.png", SerraEngine::ColorRGBA8(255,255,255,255));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameplayScreen::onExit() {
	m_debugRenderer.dispose();
}

void GameplayScreen::update() {
	m_camera.update();
	m_camera.setPosition(glm::vec2(m_player.getCapsule().getBody()->GetPosition().x, m_camera.getPosition().y));
	checkInput();
	m_player.update(m_game->getInputManager());

	m_world->Step(1.0f / 60.0f, 6, 2);

	static int counter = 0;
	if (counter >= 10) std::cout << m_game->getFPS() << std::endl, counter = 0;
	else counter++;
	
}

void GameplayScreen::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	m_textureProgram.bind();

	auto textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	auto projectionMatrix = m_camera.getCameraMatrix();
	auto pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();
	for (auto &b : m_boxes) b.draw(m_spriteBatch);
	m_player.draw(m_spriteBatch);
	m_spriteBatch.end();
	m_spriteBatch.renderBatches();

	//Debug rendering
	if (m_renderDebugMode) {
		glm::vec4 destRect;
		for (auto &b : m_boxes) {
			destRect = glm::vec4(b.getBody()->GetPosition().x - b.getDimensions().x*0.5f, b.getBody()->GetPosition().y - b.getDimensions().y*0.5f, b.getDimensions().x, b.getDimensions().y);
			m_debugRenderer.drawBox(destRect, SerraEngine::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());
			//m_debugRenderer.drawCircle(glm::vec2(b.getBody()->GetPosition().x, b.getBody()->GetPosition().y), SerraEngine::ColorRGBA8(255, 255, 255, 255), b.getDimensions().x*0.5f);
		}
		m_player.drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}

	m_textureProgram.unbind();

	//Render lights
	Light playerLight;
	playerLight.color = SerraEngine::ColorRGBA8(255, 255, 255, 50);
	playerLight.position = m_player.getPosition();
	playerLight.size = 30.0f;

	Light mouseLight;
	mouseLight.color = SerraEngine::ColorRGBA8(50, 50, 255, 150);
	mouseLight.position = m_camera.convertScreenToWorld(m_game->getInputManager().getMouseCoords());
	mouseLight.size = 35.0f;

	m_lightProgram.bind();

	pUniform = m_lightProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Additive blending

	m_spriteBatch.begin();
	playerLight.draw(m_spriteBatch);
	mouseLight.draw(m_spriteBatch);
	m_spriteBatch.end();
	m_spriteBatch.renderBatches();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Reset regular alpha blending

	m_lightProgram.unbind();
}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}