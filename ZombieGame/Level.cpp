#include "Level.h"
#include <SerraEngine/ErrorManager.h>
#include <SerraEngine/ResourceManager.h>
#include <fstream>

const int Level::TILE_WIDTH = 64;

Level::Level(const std::string& fileName) : m_numHumans(0)
{
	std::ifstream file;
	file.open(fileName);
	if (file.fail()) SerraEngine::fatalError("Failed to open " + fileName);

	std::string tmp;
	file >> tmp >> m_numHumans;
	std::getline(file, tmp); //ignore first line
	while (std::getline(file, tmp)) m_lvlData.push_back(tmp);
	
	m_lvlBatch.init();
	m_lvlBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (unsigned y = 0; y <  m_lvlData.size(); y++) {
		for (unsigned x = 0; x < m_lvlData[y].size(); x++) {
			auto tile = m_lvlData[y][x]; //grab tile
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile) //process tile
			{
			case 'B':
			case 'R':
				m_lvlBatch.pushBatch(destRect,
								  uvRect, 
								  SerraEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 
								  0.0f, 
								  SerraEngine::ColorRGBA8(255,255,255,255));
				break;
			case 'G':
				m_lvlBatch.pushBatch(destRect,
								uvRect,
								SerraEngine::ResourceManager::getTexture("Textures/glass.png").id,
								0.0f,
								SerraEngine::ColorRGBA8(255, 255, 255, 255));
				break;
			case 'L':
				m_lvlBatch.pushBatch(destRect,
								uvRect,
								SerraEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
								0.0f,
								SerraEngine::ColorRGBA8(255, 255, 255, 255));
				break;
			case '@':
				m_lvlData[y][x] = '.'; //don't collide with @
				m_playerStartPos = { x*TILE_WIDTH, y*TILE_WIDTH };
				break;
			case 'Z':
				m_lvlData[y][x] = '.'; //don't collide with Z
				m_zombiesStartPositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)\n", tile, x, y);
				break;
			}
		}
	}

	m_lvlBatch.end();
}

void Level::draw() {
	m_lvlBatch.renderBatches();
}
