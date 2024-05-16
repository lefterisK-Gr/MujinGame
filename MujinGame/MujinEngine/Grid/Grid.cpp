#include "Grid.h"

Grid::Grid(int width, int height, int cellSize)
	: _width(width), _height(height), _cellSize(cellSize)
{
	_numXCells = ceil((float)_width / _cellSize);
	_numYCells = ceil((float)_height / _cellSize);

	//Allocate all the calls
	_cells.resize(_numYCells * _numXCells);
}

Grid::~Grid()
{
}

Cell& Grid::getCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= _numXCells) x = _numXCells;
	if (y < 0) y = 0;
	if (y >= _numYCells) y = _numYCells;


	return _cells[y * _numXCells + _numXCells];
}
