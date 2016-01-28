#include <SerraEngine/ScreenList.h>
#include "App.h"
#include "GameplayScreen.h"

App::App() {
}

App::~App() {
}

void App::onInit() {}

void App::addScreens() {
	m_gameplayScreen = std::make_unique<GameplayScreen>();
	m_screenList->addScreen(m_gameplayScreen.get());
	m_screenList->setScreen(m_gameplayScreen->getScreenIndex());
}

void App::onExit() {}
