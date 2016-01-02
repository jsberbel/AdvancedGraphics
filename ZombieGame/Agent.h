#pragma once
#include <glm/glm.hpp>
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/Vertex.h>

class Zombie;
class Human;

class Agent
{
protected:
	glm::vec2 _position;
	float _speed;
	SerraEngine::Color _color;

	static const float AGENT_WIDTH;
public:
	Agent() = default;
	Agent(const glm::vec2 & position, float speed, const SerraEngine::Color& color);
	virtual ~Agent() = default;
	void pushBatch(SerraEngine::SpriteBatch & spriteBatch);
	virtual void update() = 0;

	//getters
	inline const glm::vec2 &getPosition() const { return _position; };
	inline float getSpeed() const { return _speed; };

	//setters
	//inline void setPosition(const glm::vec2 &position) const { _position = position; };
	//inline void setSpeed(float speed) const { return _speed = speed; };
};