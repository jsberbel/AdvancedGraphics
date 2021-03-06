#pragma once
#include <SerraEngine/SpriteBatch.h>
#include <SerraEngine/Vertex.h>

class Zombie;
class Human;

class Agent
{
protected:
	glm::vec2 m_position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
	float m_speed;
	SerraEngine::ColorRGBA8 m_color;
	float m_health;
	int m_textureID;

	static glm::vec2 Nlerp(const glm::vec2 &start, const glm::vec2 &end, float count);
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2> &collideTilePositions, float x, float y) const;
	void AABBCollideWithTile(glm::vec2 tilePos);
public:
	static const float AGENT_WIDTH;
	static const float AGENT_RADIUS;

	explicit Agent() = default;
	explicit Agent(const glm::vec2 &position, float speed, const SerraEngine::ColorRGBA8 &color = SerraEngine::ColorRGBA8{ 255, 255, 255, 255 });
	virtual ~Agent() = default;

	virtual void update(float deltaTime = 0.0f,
						const std::vector<std::string>& lvlData = std::vector<std::string>(),
						const std::vector<Human*>& humans = std::vector<Human*>()) = 0;
	void pushBatch(SerraEngine::SpriteBatch &spriteBatch) const;
	bool collideWithLevel(const std::vector<std::string>& lvlData, bool dirCol);
	bool collideWithAgent(Agent* agent);
	bool applyDamage(int damage); //return true if dead

	//getters
	const glm::vec2 &getPosition() const { return m_position; };
	float getSpeed() const { return m_speed; };

	//setters
	//inline void setPosition(const glm::vec2 &position) { _position = position; };
	//inline void setSpeed(float speed) const { return _speed = speed; };
};