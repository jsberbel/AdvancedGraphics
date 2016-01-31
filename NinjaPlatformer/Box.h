#pragma once
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <SerraEngine/Vertex.h>
#include <SerraEngine/SpriteBatch.h>

class Box
{
public:
	explicit Box() = default;
	explicit Box(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions, const std::string texturePath, 
				 SerraEngine::ColorRGBA8 color, bool fixedRotation = false, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	~Box();

	void init(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions, const std::string texturePath, 
			  SerraEngine::ColorRGBA8 color, bool fixedRotation = false, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	void draw(SerraEngine::SpriteBatch &spriteBatch) const;

	b2Body* getBody() const{ return m_body; };
	b2Fixture* getFixture() const { return m_fixture; };
	const glm::vec2& getDimensions() const { return m_dimensions; };
	const SerraEngine::ColorRGBA8& getColor() const { return m_color; };
private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	SerraEngine::ColorRGBA8 m_color;
	GLuint m_textureID = 0;
	glm::vec4 m_uvRect;
};

