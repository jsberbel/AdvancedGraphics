#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <SerraEngine/ResourceManager.h>
#include <SerraEngine/IMainGame.h>

void GameplayScreen::checkInput() const {
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}

GameplayScreen::GameplayScreen() {
}

GameplayScreen::~GameplayScreen() {
}

void GameplayScreen::build() {
	
}

void GameplayScreen::destroy() {
	
}

void GameplayScreen::onEntry() {
	b2Vec2 gravity(0.0f, -9.8f);
	m_world = std::make_unique<b2World>(gravity);

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	m_boxes.emplace_back(m_world.get(), glm::vec2(0.0f, 14.0f), glm::vec2(15.0f, 15.0f)); //create new box

	m_spriteBatch.init();

	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void GameplayScreen::onExit() {
	
}

void GameplayScreen::update() {
	std::cout << "Update" << std::endl;
	checkInput();
}

void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	m_spriteBatch.begin();

	m_spriteBatch.end();
	m_spriteBatch.renderBatches();
}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPrevScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}


