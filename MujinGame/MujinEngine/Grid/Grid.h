#pragma once

#include "../ECS/ECS.h"
#include "../ECS/Components.h"

#include <vector>

class Cell {
	std::vector<Entity*> entities;
};

class Grid {
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	void addEntity(Entity* entity);
	Cell& getCell(int x, int y);
	Cell& getCell(const glm::vec2& pos);


private:
	std::vector<Cell> _cells; //cache friendly
	int _cellSize;
	int _width;
	int _height;
	int _numXCells;
	int _numYCells;
};