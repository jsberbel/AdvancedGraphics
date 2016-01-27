#include "Grid.h"

Grid::Grid(int width, int height, int cellSize) : 
	m_width(width),
	m_height(height),
	m_cellSize(cellSize) {
	m_numXCells = static_cast<int>(ceil(static_cast<float>(m_width / m_cellSize)));
	m_numYCells = static_cast<int>(ceil(static_cast<float>(m_height / m_cellSize)));
	const int BALLS_TO_RESERVE = 20;
	m_cells.resize(m_numYCells*m_numXCells); //allocate cells
	for (auto &c : m_cells) c.balls.reserve(BALLS_TO_RESERVE);
}


Grid::~Grid() {

}

void Grid::addBall(Ball * ball) {
	auto* cell = getCell(ball->position);
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}

void Grid::addBall(Ball* ball, Cell * cell) const {
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}

void Grid::removeBallFromCell(Ball * ball) {
	auto& rBalls = ball->ownerCell->balls;
	rBalls[ball->cellVectorIndex] = rBalls.back();
	rBalls.pop_back();
	if (static_cast<unsigned>(ball->cellVectorIndex) < rBalls.size()) rBalls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;
}

Cell* Grid::getCell(int x, int y) {
	if (x < 0) x = 0;
	else if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	else if (y >= m_numYCells) y = m_numYCells - 1;
	return &m_cells[y*m_numXCells + x];
}

Cell* Grid::getCell(const glm::vec2 & pos) {
	auto cellX = static_cast<int>(pos.x / m_cellSize);
	auto cellY = static_cast<int>(pos.y / m_cellSize);
	return getCell(cellX, cellY);
}
