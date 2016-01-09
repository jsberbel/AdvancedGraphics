#include "Agent.h"
#include <SerraEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>

const float Agent::AGENT_WIDTH = 60.0f;

void Agent::checkTilePosition(std::vector<glm::vec2> &ctPositions, float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
									floor(y / (float)TILE_WIDTH));
	if ((*_lvlData)[(unsigned)cornerPos.y][(unsigned)cornerPos.x] != '.') {
		ctPositions.push_back(cornerPos*(float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH/2.0f));
	}
}

void Agent::AABBCollideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);
	
	if (xDepth > 0 || yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0) _position.x -= xDepth;
			else _position.x += xDepth;
		}
		else {
			if (distVec.y < 0) _position.y -= yDepth;
			else _position.y += yDepth;
		}
	}
}

Agent::Agent(const glm::vec2 & position, float speed, const SerraEngine::Color& color) :
	_position(position),
	_speed(speed),
	_color(color),
	_lvlData(nullptr),
	_humans(nullptr),
	_zombies(nullptr) {}

Agent::~Agent() {
	delete _lvlData;
	delete _humans;
	delete _zombies;
}

void Agent::initData(std::vector<std::string>& lvlData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies) {
	_lvlData = &lvlData;
	_humans = &humans;
	_zombies = &zombies;
}

void Agent::pushBatch(SerraEngine::SpriteBatch & spriteBatch) {
	glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = SerraEngine::ResourceManager::getTexture("Textures/circle.png").id;
	spriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, _color);
}

bool Agent::collideWithLevel(bool dirCol) {
	std::vector<glm::vec2> collideTilePositions;
	
	//calc direction to avoid stop bug
	if (dirCol) {
		checkTilePosition(collideTilePositions, _position.x, _position.y); //first corner
		checkTilePosition(collideTilePositions, _position.x + AGENT_WIDTH, _position.y); //second corner
		checkTilePosition(collideTilePositions, _position.x, _position.y + AGENT_WIDTH); //third corner
		checkTilePosition(collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH); //fourth corner
	} else {
		checkTilePosition(collideTilePositions, _position.x, _position.y + AGENT_WIDTH); //third corner
		checkTilePosition(collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH); //fourth corner
		checkTilePosition(collideTilePositions, _position.x, _position.y); //first corner
		checkTilePosition(collideTilePositions, _position.x + AGENT_WIDTH, _position.y); //second corner
	}
	if (collideTilePositions.size() == 0) return false;

	for (glm::vec2 ctp : collideTilePositions) AABBCollideWithTile(ctp);

	return true;
}

void Agent::collideWithAgents() {
	
}
