#pragma once
#include "Agent.h"

class Human : public Agent
{
	glm::vec2 _direction;
	int _frames;
	static const int MAX_FRAMES;
public:
	explicit Human() = default;
	explicit Human(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 30, 125, 20, 255 });
	virtual ~Human() = default;
	virtual void update(float deltaTime = 0.0f,
						const std::vector<std::string>& lvlData = std::vector<std::string>(),
						std::vector<Human*>& humans = std::vector<Human*>()) override;
};

