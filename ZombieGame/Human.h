#pragma once
#include "Agent.h"

class Human : public Agent
{
	glm::vec2 _direction;
	int _frames;
	static const int MAX_FRAMES;
public:
	Human() = default;
	Human(const glm::vec2 & position, float speed, const SerraEngine::Color &color = { 30, 125, 20, 255 });
	virtual ~Human() = default;
	virtual void update(const std::vector<std::string>& lvlData = std::vector<std::string>(),
						std::vector<Human*>& humans = std::vector<Human*>(),
						std::vector<Zombie*>& zombies = std::vector<Zombie*>()) override;
};

