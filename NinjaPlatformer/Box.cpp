#include "Box.h"
#include <SerraEngine/ResourceManager.h>

Box::Box(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const std::string texturePath, 
		 SerraEngine::ColorRGBA8 color, bool fixedRotation, glm::vec4 uvRect) {
	init(world, position, dimensions, texturePath, color, fixedRotation, uvRect);
}

Box::~Box() {
}

void Box::init(b2World * world, const glm::vec2 & position, const glm::vec2 & dimensions, const std::string texturePath, 
			   SerraEngine::ColorRGBA8 color, bool fixedRotation, glm::vec4 uvRect) {
	m_dimensions = dimensions;
	m_textureID = SerraEngine::ResourceManager::getTexture(texturePath).id;
	m_color = color;
	m_uvRect = uvRect;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x*0.5f, dimensions.y*0.5f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::draw(SerraEngine::SpriteBatch& spriteBatch) const {
	glm::vec4 destRect(m_body->GetPosition().x - m_dimensions.x*0.5f, m_body->GetPosition().y - m_dimensions.y*0.5f, m_dimensions.x, m_dimensions.y);
	spriteBatch.pushBatch(destRect, m_uvRect, m_textureID, 0.0f, m_color, m_body->GetAngle());
}
