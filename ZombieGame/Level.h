#pragma once
#include <string>
#include <vector>
#include <list>
#include <SerraEngine/SpriteBatch.h>

class Level
{
	int _numHumans;
	std::vector<std::string> _lvlData;
	SerraEngine::SpriteBatch _lvlBatch;

	glm::vec2 _playerStartPos;
	std::list<glm::vec2> _zombiesStartPositions;
public:
	static const int TILE_WIDTH;

	Level(const std::string& fileName);
	~Level() = default;
	void draw();

	//getters
	inline int getWidth() const { return _lvlData[0].size(); };
	inline int getHeight() const { return _lvlData.size(); };
	inline int getNumHumans() const { return _numHumans; };
	inline std::vector<std::string> &getLevelData() { return _lvlData; };
	inline glm::vec2 getStartPlayerPos() const { return _playerStartPos; };
	inline const std::list<glm::vec2> &getStartZombiesPos() const { return _zombiesStartPositions; };
};

