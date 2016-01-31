#pragma once
#include <SerraEngine/IMainGame.h>
#include "GameplayScreen.h"

class App : public SerraEngine::IMainGame
{
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
public:
	App();
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;
};

