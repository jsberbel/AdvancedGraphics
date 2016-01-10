#include "Agent.h"
#include <SerraEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>

const float Agent::AGENT_WIDTH = 60.0f;
const float Agent::AGENT_RADIUS = Agent::AGENT_WIDTH / 2.0f;

Agent::Agent(const glm::vec2 & position, float speed, const SerraEngine::Color &color) :
	_position(position),
	_speed(speed),
	_color(color) {}

void Agent::pushBatch(SerraEngine::SpriteBatch & spriteBatch) {
	glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = SerraEngine::ResourceManager::getTexture("Textures/circle.png").id;
	spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, _color);
}

bool Agent::collideWithLevel(const std::vector<std::string>& lvlData, bool dirCol) {
	std::vector<glm::vec2> collideTilePositions;
	
	//calc direction to avoid stop bug
	if (dirCol) {
		checkTilePosition(lvlData, collideTilePositions, _position.x, _position.y); //first corner
		checkTilePosition(lvlData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y); //second corner
		checkTilePosition(lvlData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH); //third corner
		checkTilePosition(lvlData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH); //fourth corner
	} else {
		checkTilePosition(lvlData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH); //third corner
		checkTilePosition(lvlData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH); //fourth corner
		checkTilePosition(lvlData, collideTilePositions, _position.x, _position.y); //first corner
		checkTilePosition(lvlData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y); //second corner
	}
	if (!collideTilePositions.size()) return false;

	for (const glm::vec2 &ctp : collideTilePositions) AABBCollideWithTile(ctp);

	return true;
}

bool Agent::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS*2.0f; // Minimum distance before collision

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS); // Center position of this agent
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS); // Center position of the parameter agent
	glm::vec2 distVec = centerPosA - centerPosB; // Distance vector between the two agents

	float distance = glm::length(distVec); // Length of the distance vector
	float collisionDepth = MIN_DISTANCE - distance; // Depth of the collision
	if (collisionDepth > 0) {
		glm::vec2 collisionDepthVec = glm::normalize(distVec)*collisionDepth; // Get direction times collision depth so as to push away from each other
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

bool Agent::applyDamage(float damage) {
	_health -= damage;
	if (_health <= 0) return true;
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& lvlData, std::vector<glm::vec2> &collideTilePositions, float x, float y) {
	// Get position of parameter corner in grid-space
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)Level::TILE_WIDTH),
									floor(y / (float)Level::TILE_WIDTH));

	if (cornerPos.x < 0 || cornerPos.x >= lvlData[0].size() ||
		cornerPos.y < 0 || cornerPos.y >= lvlData.size()) return;

	// If is not a ground tile, push corner position to vector
	if (lvlData[(unsigned)cornerPos.y][(unsigned)cornerPos.x] != '.') {
		collideTilePositions.push_back(cornerPos*(float)Level::TILE_WIDTH + glm::vec2((float)Level::TILE_WIDTH / 2.0f));
	}
}

void Agent::AABBCollideWithTile(glm::vec2 tilePos) {
	const float TILE_RADIUS = (float)Level::TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS); // Center position of the agent
	glm::vec2 distVec = centerPlayerPos - tilePos; // Vector from the agent to the tile
	// Get collision depth
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		// Check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X collsion depth is smaller so we push in X direction
			if (distVec.x < 0) _position.x -= xDepth;
			else _position.x += xDepth;
		}
		else {
			// Y collsion depth is smaller so we push in X direction
			if (distVec.y < 0) _position.y -= yDepth;
			else _position.y += yDepth;
		}
	}
}
