#pragma once

#include "Components.h"

#define GRID_EMPTY_TILE 0x0000
#define GRID_SOLID_TILE 0x0001
#define GRID_ROWS 4
#define GRID_COLUMNS 4
#define GRID_ELEMENT_WIDTH 8
#define GRID_ELEMENT_HEIGHT 8
#define TILE_NUM_GRID_ELEMENTS (GRID_ROWS * GRID_COLUMNS) //16
#define MAX_MAP_TILE_HEIGHT 20
#define MAX_MAP_WIDTH_TILES 100
#define MAX_MAP_GRID_HEIGHT (MAX_MAP_TILE_HEIGHT * GRID_ROWS) //80
#define MAX_MAP_GRID_WIDTH (MAX_MAP_TILE_HEIGHT * GRID_COLUMNS) //100

class GridComponent : public Component //GridComp --> ColliderComp
{
public:
	Vector2D position;
	int scaledTile;
	ColliderComponent* collider = nullptr;

	GridComponent() = default;

	~GridComponent()
	{

	}

	GridComponent(int xpos, int ypos, int tscaled)
	{
		position.x = xpos;
		position.y = ypos;

		scaledTile = tscaled;
	}

	void init() override
	{
		Vector2D gridPos;

		for (auto gridindex = 0; gridindex < TILE_NUM_GRID_ELEMENTS; gridindex++)
		{ //SetGridTileBlock
			gridPos.x = (gridindex % GRID_COLUMNS) * GRID_ELEMENT_WIDTH;
			gridPos.y = (int)(gridindex / GRID_ROWS) * GRID_ELEMENT_HEIGHT;

			entity->addComponent<ColliderComponent>("terrain", (position.x + gridPos.x), (position.y + gridPos.y), GRID_ELEMENT_WIDTH);
		}
	}

	void update(float deltaTime) override
	{
		
	}

	void updateCollidersGrid() {
		int gridindex = 0;

		for (auto& ccomp : entity->components) { // get all the ColliderComponents

			ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

			if (!colliderComponentPtr) {
				continue;
			}
			glm::vec2 gridPos;
			gridPos.x = (gridindex % GRID_COLUMNS) * GRID_ELEMENT_WIDTH;
			gridPos.y = (int)(gridindex / GRID_ROWS) * GRID_ELEMENT_HEIGHT;

			colliderComponentPtr->updateCollider(gridPos);

			gridindex++;
		}
	}
};
