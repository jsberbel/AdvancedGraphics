#pragma once
#include <string>
#include <vector>
#include <SerraEngine/SpriteBatch.h>

#define TILE_WIDTH 64

class Level
{
	std::vector<std::string> _lvlData;
	int _numHumans;
	SerraEngine::SpriteBatch _spriteBatch;
	glm::ivec2 _startPlayerPos;
	std::vector<glm::ivec2> _zombiesStartPosition;
public:
	Level(const std::string& fileName);
	~Level();
	void draw();
};

