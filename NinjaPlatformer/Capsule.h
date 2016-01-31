#pragma once
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <SerraEngine/DebugRenderer.h>

class Capsule {
public:
	explicit Capsule() = default;
	~Capsule() = default;

	void init(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions, float density, float friction, bool fixedRotation = false);
	void drawDebug(SerraEngine::DebugRenderer& debugRenderer);

	b2Body* getBody() const { return m_body; };
	b2Fixture* getFixture(int index) const { return m_fixtures[index]; };
	const glm::vec2& getDimensions() const { return m_dimensions; };
private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dimensions;
};

