#pragma once
#include <vector>
#include "Ball.h"

struct Cell
{
	std::vector<Ball*> balls;
};

class Grid
{
	int m_width;
	int m_height;
	int m_cellSize;
	int m_numXCells;
	int m_numYCells;
	std::vector<Cell> m_cells;
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	void addBall(Ball* ball);
	void addBall(Ball* ball, Cell* cell) const;
	void removeBallFromCell(Ball* ball);
	Cell* getCell(int x, int y);
	Cell* getCell(const glm::vec2 &pos);

	friend class BallController;
};

