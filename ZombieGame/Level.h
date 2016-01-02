#pragma once
#include <string>
#include <vector>
#include <SerraEngine/SpriteBatch.h>

#define TILE_WIDTH 64

class Level
{
	int _numHumans;
	std::vector<std::string> _lvlData;
	SerraEngine::SpriteBatch _lvlBatch;

	glm::vec2 _playerStartPos;
	std::vector<glm::vec2> _zombiesStartPosition;
public:
	Level(const std::string& fileName);
	~Level() = default;
	void draw();

	//getters
	inline glm::vec2 getStartPlayerPos() const { return _playerStartPos; };
	inline const std::vector<glm::vec2> &getStartZombiePos() const { return _zombiesStartPosition; };
};

