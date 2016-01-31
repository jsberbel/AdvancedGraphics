#include "Capsule.h"

void Capsule::init(b2World * world, const glm::vec2 & position, const glm::vec2 & dimensions, float density, float friction, bool fixedRotation) {
	m_dimensions = dimensions;

	//Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	//Make the box
	b2PolygonShape boxShape;
	boxShape.SetAsBox(m_dimensions.x*0.5f, (m_dimensions.y- m_dimensions.x)*0.5f);

	b2FixtureDef boxDef;
	boxDef.shape = &boxShape;
	boxDef.density = density;
	boxDef.friction = friction;
	m_fixtures[0] = m_body->CreateFixture(&boxDef);

	//Make the circles
	b2CircleShape circleShape;
	circleShape.m_radius = m_dimensions.x*0.5f;

	b2FixtureDef cirleDef;
	cirleDef.shape = &circleShape;
	cirleDef.density = density;
	cirleDef.friction = friction;
	//Bottom circle
	circleShape.m_p.Set(0.0f, (-m_dimensions.y + m_dimensions.x)*0.5f);
	m_fixtures[1] = m_body->CreateFixture(&cirleDef);
	//Top circle
	circleShape.m_p.Set(0.0f, (m_dimensions.y + m_dimensions.x)*0.5f);
	m_fixtures[2] = m_body->CreateFixture(&cirleDef);
}

void Capsule::drawDebug(SerraEngine::DebugRenderer& debugRenderer) {
	SerraEngine::ColorRGBA8 color(255, 255, 255, 255);
	//Draw box
	glm::vec4 destRect(m_body->GetPosition().x - m_dimensions.x*0.5f, m_body->GetPosition().y - (m_dimensions.y - m_dimensions.x)*0.5f, m_dimensions.x, m_dimensions.y - m_dimensions.x);
	debugRenderer.drawBox(destRect, color, m_body->GetAngle());
	//Draw circles
	debugRenderer.drawCircle(glm::vec2(destRect.x + m_dimensions.x*0.5f, destRect.y), color, m_dimensions.x*0.5f);
	debugRenderer.drawCircle(glm::vec2(destRect.x + m_dimensions.x*0.5f, destRect.y + destRect.w), color, m_dimensions.x*0.5f);
}
