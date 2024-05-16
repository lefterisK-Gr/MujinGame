#pragma once
#include "../ECS/ECS.h"
#include <vector>

#include <cmath>

struct Cell {
	std::vector<Entity*> entities;
};


class Grid {
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	Cell& getCell(int x, int y);

private:
	std::vector<Cell> _cells;
	int _cellSize;
	int _width;
	int _height;
	int _numXCells;
	int _numYCells;
};