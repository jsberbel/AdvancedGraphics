#pragma once
#include <SerraEngine/Camera2D.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/DebugRenderer.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/IGameScreen.h>
#include <SerraEngine/Window.h>
#include <Box2D/Box2D.h>
#include "Box.h"
#include "Player.h"

class GameplayScreen : public SerraEngine::IGameScreen
{
public:
	explicit GameplayScreen(SerraEngine::GLWindow* window);
	~GameplayScreen();

	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;
	virtual int getNextScreenIndex() const override;
	virtual int getPrevScreenIndex() const override;
private:
	void checkInput() const;

	SerraEngine::GLSLManager m_textureProgram;
	SerraEngine::GLSLManager m_lightProgram;
	SerraEngine::Camera2D m_camera;
	SerraEngine::SpriteBatch m_spriteBatch;
	std::vector<Box> m_boxes;
	std::unique_ptr<b2World> m_world;
	SerraEngine::GLWindow* m_window = nullptr;
	Player m_player;
	SerraEngine::DebugRenderer m_debugRenderer;
	bool m_renderDebugMode = false;
};

