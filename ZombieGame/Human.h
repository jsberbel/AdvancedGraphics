#pragma once
#include "Agent.h"

class Human : public Agent
{
	const int MAX_FRAMES = 100;
	int m_frames;
	glm::vec2 m_startDir = glm::vec2(1.0f, 0.0f);
	glm::vec2 m_endDir = glm::vec2(1.0f, 0.0f);
	float m_countDir = 0.0f;
public:
	explicit Human() = default;
	explicit Human(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 255, 255, 255, 255 });
	virtual ~Human() = default;
	virtual void update(float deltaTime = 0.0f,
						const std::vector<std::string>& lvlData = std::vector<std::string>(),
						const std::vector<Human*>& humans = std::vector<Human*>()) override;
};

