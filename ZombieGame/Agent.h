#pragma once
#include <glm/glm.hpp>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/Vertex.h>
#include <memory>

class Zombie;
class Human;

class Agent
{
protected:
	glm::vec2 _position;
	float _speed;
	SerraEngine::Color _color;
	float _health;

	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2> &collideTilePositions, float x, float y);
	void AABBCollideWithTile(glm::vec2 tilePos);
public:
	static const float AGENT_WIDTH;
	static const float AGENT_RADIUS;

	Agent() = default;
	Agent(const glm::vec2 &position, float speed, const SerraEngine::Color &color = { 255, 255, 255, 255 });
	virtual ~Agent() = default;

	virtual void update(const std::vector<std::string>& lvlData = std::vector<std::string>(),
						std::vector<Human*>& humans = std::vector<Human*>(),
						std::vector<Zombie*>& zombies = std::vector<Zombie*>()) = 0;
	void pushBatch(SerraEngine::SpriteBatch &spriteBatch);
	bool collideWithLevel(const std::vector<std::string>& lvlData, bool dirCol);
	bool collideWithAgent(Agent* agent);
	bool applyDamage(float damage); //return true if dead

	//getters
	inline const glm::vec2 &getPosition() const { return _position; };
	inline float getSpeed() const { return _speed; };

	//setters
	//inline void setPosition(const glm::vec2 &position) { _position = position; };
	//inline void setSpeed(float speed) const { return _speed = speed; };
};