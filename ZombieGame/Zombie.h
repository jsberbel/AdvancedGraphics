#pragma once
#include "Agent.h"
#include <list>

class Zombie : public Agent
{
	Human* getNearestHuman(std::vector<Human*>& humans);
public:
	Zombie() = default;
	Zombie(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color = { 180, 0, 0, 255 });
	~Zombie() = default;
	void update(float deltaTime = 0,
				const std::vector<std::string>& lvlData = std::vector<std::string>(),
				std::vector<Human*>& humans = std::vector<Human*>(),
				std::vector<Zombie*>& zombies = std::vector<Zombie*>()) override;
};

