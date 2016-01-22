#pragma once
#include <vector>
#include <list>
#include <SerraEngine/SpriteBatch.h>

class Level
{
	int m_numHumans;
	std::vector<std::string> m_lvlData;
	SerraEngine::SpriteBatch m_lvlBatch;

	glm::vec2 m_playerStartPos;
	std::list<glm::vec2> m_zombiesStartPositions;
public:
	static const int TILE_WIDTH;

	explicit Level(const std::string& fileName);
	~Level() = default;
	void draw();

	//getters
	int getWidth() const { return m_lvlData[0].size(); };
	int getHeight() const { return m_lvlData.size(); };
	int getNumHumans() const { return m_numHumans; };
	std::vector<std::string>& getLevelData() { return m_lvlData; };
	glm::vec2 getStartPlayerPos() const { return m_playerStartPos; };
	const std::list<glm::vec2>& getStartZombiesPos() const { return m_zombiesStartPositions; };
};

