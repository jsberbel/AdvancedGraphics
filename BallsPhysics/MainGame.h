#pragma once

#include <SerraEngine/Camera2D.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/InputManager.h>
#include <SerraEngine/Window.h>
#include <SerraEngine/GLSLManager.h>
#include <SerraEngine/Timing.h>
#include <SerraEngine/SpriteFont.h>
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

enum class GameState { RUNNING, EXIT };

class MainGame {
public:
	MainGame() = default;
	~MainGame() = default;
    void run();

private:
    void init();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls
	std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collisions

    BallController m_ballController; ///< Controls balls
    BallRenderer m_ballRenderer; ///< Renders balls

	SerraEngine::GLWindow m_window; ///< The main window
	SerraEngine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<SerraEngine::SpriteFont> m_spriteFont; ///< For font rendering
	SerraEngine::Camera2D m_camera; ///< Renders the scene
	SerraEngine::InputManager m_inputManager; ///< Handles input
	SerraEngine::GLSLManager m_textureProgram; ///< Shader for textures]

	SerraEngine::FPSLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

