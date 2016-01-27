#pragma once
#include "Agent.h"

class Zombie : public Agent
{
	glm::vec2 m_startDir = glm::vec2(1.0f, 0.0f);
	glm::vec2 m_endDir = glm::vec2(1.0f, 0.0f);
	float m_countDir = 0.0f;
	Human* m_prevHuman = nullptr;
	Human* getNearestHuman(const std::vector<Human*>& humans) const;
public:
	explicit Zombie() = default;
	explicit Zombie(const glm::vec2 & position, float speed, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 255, 255, 255, 255 });
	~Zombie() = default;
	void update(float deltaTime = 0.0f,
				const std::vector<std::string>& lvlData = std::vector<std::string>(),
				const std::vector<Human*>& humans = std::vector<Human*>()) override;
};

