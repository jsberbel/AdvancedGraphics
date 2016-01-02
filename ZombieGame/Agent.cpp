#include "Agent.h"
#include <SerraEngine/ResourceManager.h>

const float Agent::AGENT_WIDTH = 60.0f;

Agent::Agent(const glm::vec2 & position, float speed, const SerraEngine::Color& color) :
	_position(position),
	_speed(speed),
	_color(color) {}

void Agent::pushBatch(SerraEngine::SpriteBatch & spriteBatch) {
	glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = SerraEngine::ResourceManager::getTexture("Textures/circle.png").id;
	spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, _color);
}
