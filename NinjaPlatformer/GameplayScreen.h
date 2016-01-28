#pragma once
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include <vector>
#include "Box.h"

class GameplayScreen : public SerraEngine::IGameScreen
{
	SerraEngine::GLSLManager m_textureProgram;
	SerraEngine::Camera2D m_camera;
	SerraEngine::SpriteBatch m_spriteBatch;
	std::vector<Box> m_boxes;
	std::unique_ptr<b2World> m_world;

	void checkInput() const;
public:
	GameplayScreen();
	~GameplayScreen();

	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;
	virtual int getNextScreenIndex() const override;
	virtual int getPrevScreenIndex() const override;
};

