#pragma once
#include "Agent.h"

class Human : public Agent
{
public:
	Human() = default;
	Human(const glm::vec2 & position, float speed, const SerraEngine::Color& color);
	virtual ~Human();
	virtual void update() = 0;
};

