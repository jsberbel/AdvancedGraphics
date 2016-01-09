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
	static const float AGENT_WIDTH;

	glm::vec2 _position;
	float _speed;
	SerraEngine::Color _color;
	std::vector<std::string> *_lvlData;
	std::vector<Human*> *_humans;
	std::vector<Zombie*> *_zombies;
		
	void checkTilePosition(std::vector<glm::vec2> &ctPositions, float x, float y);
	void AABBCollideWithTile(glm::vec2 tilePos);
public:
	Agent() = default;
	Agent(const glm::vec2 &position, float speed, const SerraEngine::Color &color);
	~Agent();

	void initData(std::vector<std::string>& lvlData, std::vector<Human*> &humans, std::vector<Zombie*> &zombies);
	virtual void update() = 0;
	void pushBatch(SerraEngine::SpriteBatch &spriteBatch);
	bool collideWithLevel(bool dirCol);
	void collideWithAgents();

	//getters
	inline const glm::vec2 &getPosition() const { return _position; };
	inline float getSpeed() const { return _speed; };

	//setters
	//inline void setPosition(const glm::vec2 &position) const { _position = position; };
	//inline void setSpeed(float speed) const { return _speed = speed; };
};