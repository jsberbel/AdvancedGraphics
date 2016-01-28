#pragma once
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

class Box
{
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
public:
	explicit Box(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions);
	~Box();

	b2Body* getBody() const{ return m_body; };
	b2Fixture* getFixture() const { return m_fixture; };
};

