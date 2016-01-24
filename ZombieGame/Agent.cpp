#include "Agent.h"
#include <SerraEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>

const float Agent::AGENT_WIDTH = 60.0f;
const float Agent::AGENT_RADIUS = Agent::AGENT_WIDTH / 2.0f;

Agent::Agent(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color) :
	m_position(position),
	m_speed(speed),
	m_color(color), 
	m_health(100) {
	m_textureID = SerraEngine::ResourceManager::getTexture("Textures/circle.png").id;
}

void Agent::pushBatch(SerraEngine::SpriteBatch & spriteBatch) const
{
	glm::vec4 destRect(m_position.x, m_position.y, AGENT_WIDTH, AGENT_WIDTH);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	spriteBatch.pushBatch(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);
}

bool Agent::collideWithLevel(const std::vector<std::string>& lvlData, bool dirCol) {
	std::vector<glm::vec2> collideTilePositions;
	
	//calc direction to avoid stop bug
	if (dirCol) {
		checkTilePosition(lvlData, collideTilePositions, m_position.x, m_position.y); //first corner
		checkTilePosition(lvlData, collideTilePositions, m_position.x + AGENT_WIDTH, m_position.y); //second corner
		checkTilePosition(lvlData, collideTilePositions, m_position.x, m_position.y + AGENT_WIDTH); //third corner
		checkTilePosition(lvlData, collideTilePositions, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH); //fourth corner
	} else {
		checkTilePosition(lvlData, collideTilePositions, m_position.x, m_position.y + AGENT_WIDTH); //third corner
		checkTilePosition(lvlData, collideTilePositions, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH); //fourth corner
		checkTilePosition(lvlData, collideTilePositions, m_position.x, m_position.y); //first corner
		checkTilePosition(lvlData, collideTilePositions, m_position.x + AGENT_WIDTH, m_position.y); //second corner
	}
	if (!collideTilePositions.size()) return false;

	for (const auto& ctp : collideTilePositions) AABBCollideWithTile(ctp);

	return true;
}

bool Agent::collideWithAgent(Agent* agent) {
	// Is the Agent too far away in the X direction to check?
	if (agent->m_position.x < m_position.x - AGENT_WIDTH || agent->m_position.x > m_position.x + AGENT_WIDTH) return false;

	// Is the Agent too far away in the Y direction to check?
	if (agent->m_position.y < m_position.y - AGENT_WIDTH || agent->m_position.y > m_position.y + AGENT_WIDTH) return false;

	const auto MIN_DISTANCE = AGENT_WIDTH; // This used to be AGENT_RADIUS * 2

	auto centerPosA = m_position + glm::vec2(AGENT_RADIUS); // Center position of this agent
	auto centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS); // Center position of the parameter agent
	auto distVec = centerPosA - centerPosB; // Distance vector between the two agents

	auto distance = glm::length(distVec); // Length of the distance vector
	auto collisionDepth = MIN_DISTANCE - distance; // Depth of the collision
	if (collisionDepth > 0) {
		auto collisionDepthVec = glm::normalize(distVec)*collisionDepth; // Get direction times collision depth so as to push away from each other
		m_position += collisionDepthVec / 2.0f;
		agent->m_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

bool Agent::applyDamage(int damage) {
	m_health -= damage;
	if (m_health <= 0) return true;
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& lvlData, std::vector<glm::vec2> &collideTilePositions, float x, float y) const
{
	// Get position of parameter corner in grid-space
	auto cornerPos = glm::vec2(floor(x / static_cast<float>(Level::TILE_WIDTH)),
									floor(y / static_cast<float>(Level::TILE_WIDTH)));

	if (cornerPos.x < 0 || cornerPos.x >= lvlData[0].size() ||
		cornerPos.y < 0 || cornerPos.y >= lvlData.size()) return;

	// If is not a ground tile, push corner position to vector
	if (lvlData[static_cast<unsigned>(cornerPos.y)][static_cast<unsigned>(cornerPos.x)] != '.') {
		collideTilePositions.push_back(cornerPos*static_cast<float>(Level::TILE_WIDTH) + glm::vec2(static_cast<float>(Level::TILE_WIDTH) / 2.0f));
	}
}

void Agent::AABBCollideWithTile(glm::vec2 tilePos) {
	const auto TILE_RADIUS = static_cast<float>(Level::TILE_WIDTH) / 2.0f;
	const auto MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	auto centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS); // Center position of the agent
	auto distVec = centerPlayerPos - tilePos; // Vector from the agent to the tile
	
	// Get collision depth
	auto xDepth = MIN_DISTANCE - abs(distVec.x);
	auto yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		// Check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X collsion depth is smaller so we push in X direction
			if (distVec.x < 0) m_position.x -= xDepth;
			else m_position.x += xDepth;
		}
		else {
			// Y collsion depth is smaller so we push in X direction
			if (distVec.y < 0) m_position.y -= yDepth;
			else m_position.y += yDepth;
		}
	}
}

glm::vec2 Agent::Nlerp(const glm::vec2 &start, const glm::vec2 &end, float count) {
	return glm::normalize(start + count*(end - start));
}
