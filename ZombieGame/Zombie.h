#pragma once
#include "Agent.h"
#include <list>

class Zombie : public Agent
{
	Human* getNearestHuman(const std::vector<Human*>& humans) const;
public:
	explicit Zombie() = default;
	explicit Zombie(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 180, 0, 0, 255 });
	~Zombie() = default;
	using Agent::update;
	void update(float deltaTime = 0.0f,
				const std::vector<std::string>& lvlData = std::vector<std::string>(),
				const std::vector<Human*>& humans = std::vector<Human*>()) override;
};

