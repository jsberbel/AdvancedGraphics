#pragma once
#include "Agent.h"

class Human : public Agent
{
	const int MAX_FRAMES = 30;
	glm::vec2 m_direction;
	int m_frames;
public:
	explicit Human() = default;
	explicit Human(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 30, 125, 20, 255 });
	virtual ~Human() = default;
	virtual void update(float deltaTime = 0.0f,
						const std::vector<std::string>& lvlData = std::vector<std::string>(),
						const std::vector<Human*>& humans = std::vector<Human*>()) override;
};

