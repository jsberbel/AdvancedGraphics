#include "Level.h"
#include <SerraEngine/ErrorManager.h>
#include <SerraEngine/ResourceManager.h>
#include <fstream>

Level::Level(const std::string& fileName) {
	std::ifstream file;
	file.open(fileName);
	if (file.fail()) SerraEngine::errorRunTime("Failed to open " + fileName);

	std::string tmp;
	file >> tmp >> _numHumans;
	while (std::getline(file, tmp)) _lvlData.push_back(tmp);
	
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (unsigned y = _lvlData.size()-1; y > 0; y--) {
		for (unsigned x = 0; x < _lvlData[y].size(); x++) {
			char tile = _lvlData[y][x]; //grab tile
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile) //process tile
			{
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, 
								  uvRect, 
								  SerraEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 
								  0.0f, 
								  SerraEngine::Color(255,255,255,255));
				break;
			case 'G':
				_spriteBatch.draw(destRect,
								uvRect,
								SerraEngine::ResourceManager::getTexture("Textures/glass.png").id,
								0.0f,
								SerraEngine::Color(255, 255, 255, 255));
				break;
			case 'L':
				_spriteBatch.draw(destRect,
								uvRect,
								SerraEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
								0.0f,
								SerraEngine::Color(255, 255, 255, 255));
				break;
			case '@':
				_startPlayerPos = { x*TILE_WIDTH, y*TILE_WIDTH };
				_spriteBatch.draw(destRect,
								uvRect,
								SerraEngine::ResourceManager::getTexture("Textures/circle.png").id,
								0.0f,
								SerraEngine::Color(255, 255, 255, 255));
				break;
			case 'Z':
				_zombiesStartPosition.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				_spriteBatch.draw(destRect,
								uvRect,
								SerraEngine::ResourceManager::getTexture("Textures/circle.png").id,
								0.0f,
								SerraEngine::Color(255, 255, 255, 255));
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)\n", tile, x, y);
				break;
			}
		}
	}

	_spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw() {
	_spriteBatch.renderBatch();
}
