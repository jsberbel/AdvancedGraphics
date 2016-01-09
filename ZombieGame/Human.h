#pragma once
#include "Agent.h"

class Human : public Agent
{
	glm::vec2 _direction;
	int _frames;
	static const int MAX_FRAMES;
public:
	Human() = default;
	Human(const glm::vec2 & position, float speed, const SerraEngine::Color& color = { 255, 0, 255, 255 });
	virtual ~Human() = default;
	virtual void update();
};

