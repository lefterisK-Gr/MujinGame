#include "Grid.h"

#include <cmath>

Grid::Grid(int width, int height, int cellSize) : 
	_width(width),
	_height(height),
	_cellSize(cellSize){
	_numXCells = ceil((float)_width		/ _cellSize);
	_numYCells = ceil((float)_height	/ _cellSize);

	//allocate all the cells
	_cells.resize(_numXCells * _numYCells);

}

Grid::~Grid() {

}

void Grid::addEntity(Entity* entity)
{
	//Cell& cell = getCell(entity->GetComponent<TransformComponent>().position);
	//cell.entities.push_back(entity);

}

Cell& Grid::getCell(int x, int y){
	if (x < 0) x = 0;
	if (x > _numXCells) x = _numXCells;
	if (y < 0) y = 0;
	if (y > _numYCells) x = _numYCells;

	return _cells[y * _numXCells + _numXCells];
}

Cell& Grid::getCell(const glm::vec2& pos)
{
	int cellX = (int)(pos.x / _cellSize);
	int cellY = (int)(pos.y / _cellSize);

	return getCell(cellX, cellY);
}
