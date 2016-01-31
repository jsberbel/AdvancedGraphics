#include "Player.h"
#include <SDL/SDL.h>
#include <SerraEngine/ResourceManager.h>

Player::Player(b2World* world, const glm::vec2& position, const glm::vec2& drawDims, const glm::vec2& collisionDims, const std::string texturePath, SerraEngine::ColorRGBA8 color) {
	init(world, position, drawDims, collisionDims, texturePath, color);
}

void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& drawDims, const glm::vec2& collisionDims, const std::string texturePath, SerraEngine::ColorRGBA8 color) {
	m_drawDims = drawDims;
	m_color = color;
	m_collisionCapsule.init(world, position, collisionDims, 1.0f, 0.1f, true);
	m_texture.init(SerraEngine::ResourceManager::getTexture(texturePath).id, glm::ivec2(10,2));
}

void Player::draw(SerraEngine::SpriteBatch& spriteBatch) {
	b2Body* body = m_collisionCapsule.getBody();
	glm::vec4 destRect(body->GetPosition().x - m_drawDims.x*0.5f, 
					   body->GetPosition().y - m_collisionCapsule.getDimensions().y*0.5f, 
					   m_drawDims.x, 
					   m_drawDims.y);

	int numTiles = 0;
	int tileIndex = 0;
	float animSpeed = 0.2f;
	glm::vec2 velocity(body->GetLinearVelocity().x, body->GetLinearVelocity().y);

	//Calculate animation
	if (m_onGround) {
		if (m_punching) {
			numTiles = 4;
			tileIndex = 1;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animTime = 0.0f;
			}
		} else if (abs(velocity.x) > 1.0f && (velocity.x > 0 && m_direction > 0 || velocity.x < 0 && m_direction < 0)) {
			//Running
			numTiles = 6;
			tileIndex = 10;
			animSpeed = abs(velocity.x)*0.025f;
			if (m_moveState != PlayerMoveState::RUNNING) {
				m_moveState = PlayerMoveState::RUNNING;
				m_animTime = 0.0f;
			}
		} else {
			//Standing still
			numTiles = 1;
			tileIndex = 0;
			m_moveState = PlayerMoveState::STANDING;
		}
	} else {
		//In the air
		if (m_punching) {
			numTiles = 1;
			tileIndex = 18;
			animSpeed *= 0.25f;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animTime = 0.0f;
			}
		} else if (abs(velocity.x) > 9.0f) {
			numTiles = 1;
			tileIndex = 10;
		} else if (velocity.y <= 0.0f) {
			//Falling
			numTiles = 1;
			tileIndex = 17;
		} else {
			//Rising
			numTiles = 1;
			tileIndex = 16;
			m_moveState = PlayerMoveState::IN_AIR;
		}
	}

	m_animTime += animSpeed; //Increment animation time
	if (m_animTime > numTiles) m_punching = false;
	tileIndex = tileIndex + static_cast<int>(m_animTime) % numTiles; //Apply animation

	glm::vec4 uvRect = m_texture.getUVs(tileIndex);
	//Flip UVs according to direction
	if (m_direction == -1) {
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	spriteBatch.pushBatch(destRect, uvRect, m_texture.textureID, 0.0f, m_color, body->GetAngle());
}

void Player::drawDebug(SerraEngine::DebugRenderer& debugRenderer) {
	m_collisionCapsule.drawDebug(debugRenderer);
}

void Player::update(SerraEngine::InputManager inputManager) {
	b2Body* body = m_collisionCapsule.getBody();
	if (inputManager.isKeyDown(SDLK_a)) body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true), m_direction = -1;
	else if (inputManager.isKeyDown(SDLK_d)) body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true), m_direction = 1;
	else body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x*0.96f, body->GetLinearVelocity().y));

	if (inputManager.isKeyPressed(SDLK_SPACE)) m_punching = true;

	const float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED) body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	else if (body->GetLinearVelocity().x > MAX_SPEED) body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));

	//Loop through contact points
	m_onGround = false;
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			//Check if points are below
			auto below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - m_collisionCapsule.getDimensions().y*0.5f + m_collisionCapsule.getDimensions().x*0.5f + 0.01f) {
					below = true;
					break;
				}
			}
			if (below) {
				m_onGround = true;
				//Can jump
				if (inputManager.isKeyPressed(SDLK_w)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}
