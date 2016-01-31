#pragma once
#include <SerraEngine/Vertex.h>
#include <SerraEngine/SpriteBatch.h>
#include <glm/detail/type_vec2.hpp>

class Light {
public:
	explicit Light() = default;
	~Light() = default;

	void draw(SerraEngine::SpriteBatch &spriteBatch) {
		glm::vec4 destRect(position.x - size*0.5f, position.y - size*0.5f, size, size);
		spriteBatch.pushBatch(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0.0f);
	};

	SerraEngine::ColorRGBA8 color;
	glm::vec2 position;
	float size;
};

