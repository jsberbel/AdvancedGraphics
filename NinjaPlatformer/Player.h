#pragma once
#include <glm/detail/type_vec2.hpp>
#include <SerraEngine/InputManager.h>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/TileSheet.h>
#include "Capsule.h"

enum class PlayerMoveState { STANDING, RUNNING, PUNCHING, IN_AIR };

class Player
{
	Capsule m_collisionCapsule;
	SerraEngine::ColorRGBA8 m_color;
	SerraEngine::TileSheet m_texture;
	glm::vec2 m_drawDims;
	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
	bool m_onGround = false;
	int m_direction = 1;
	float m_animTime = 0.0f;
	bool m_punching = false;
public:
	explicit Player() = default;
	explicit Player(b2World* world, 
					const glm::vec2& position, 
					const glm::vec2& drawDims, 
					const glm::vec2& collisionDims, 
					const std::string texturePath, 
					SerraEngine::ColorRGBA8 color);
	~Player() = default;
	
	void init(b2World* world,
			  const glm::vec2& position,
			  const glm::vec2& drawDims,
			  const glm::vec2& collisionDims,
			  const std::string texturePath,
			  SerraEngine::ColorRGBA8 color);

	void draw(SerraEngine::SpriteBatch &spriteBatch);
	void drawDebug(SerraEngine::DebugRenderer& debugRenderer);
	void update(SerraEngine::InputManager inputManager);

	const Capsule &getCapsule() const { return m_collisionCapsule; };
	const glm::vec2 &getPosition() const { return glm::vec2(m_collisionCapsule.getBody()->GetPosition().x, m_collisionCapsule.getBody()->GetPosition().y); };
};
