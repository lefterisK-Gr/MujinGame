#include "Map.h"
//#include "TextureManager.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"

#define GRID_ROWS 4
#define GRID_COLUMNS 4
#define GRID_ELEMENT_WIDTH 8
#define GRID_ELEMENT_HEIGHT 8
#define TILE_NUM_GRID_ELEMENTS (GRID_ROWS * GRID_COLUMNS) //16
#define MAX_MAP_TILE_HEIGHT 20
#define MAX_MAP_WIDTH_TILES 100
#define MAX_MAP_GRID_HEIGHT (MAX_MAP_TILE_HEIGHT * GRID_ROWS) //80
#define MAX_MAP_GRID_WIDTH (MAX_MAP_TILE_HEIGHT * GRID_COLUMNS) //100

extern Manager manager;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) //probably initiallization
{
	scaledSize = ms * ts;
}

Map::~Map()
{

}

void Map::LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath, std::string actionlayerpath, std::string foregroundpath)
{
	bool isSolid = false;
	bool isAnimated = false;
	bool isWinning = false;
	bool isMysteryBox = false;
	char c;
	int x = 0, y = 0;
	int wordNum = 0;
	int arrayTilesIndex = 0;

	int i;
	int srcX, srcY;

	Vector2D gridPos;

	int mapgridIndex = 0;

	std::string line, word;
	std::fstream mapFile;
	mapFile.open(actionlayerpath);

	while (getline(mapFile, line)) //reading tiles (action layer)
	{
		std::stringstream str(line);

		while (getline(str, word, ',')) //this is searching in tilemap
		{
			wordNum = stoi(word);
			srcY = (wordNum / 26) * tileSize;
			srcX = (wordNum % 26) * tileSize; //adding tile based on srcX,srcY coordinates
			
			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_SOLID_TILES; arrayTilesIndex++)
			{
				if (wordNum == solidTiles[arrayTilesIndex])
				{
					isSolid = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_MYSTERY_BOX_TILES; arrayTilesIndex++)
			{
				if (wordNum == mysteryBoxTiles[arrayTilesIndex])
				{
					isMysteryBox = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_ANIMATED_TILES; arrayTilesIndex++)
			{
				if (wordNum == animatedTiles[arrayTilesIndex])
				{
					isAnimated = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_WINNING_TILES; arrayTilesIndex++)
			{
				if (wordNum == winningTiles[arrayTilesIndex])
				{
					isWinning = true;
				}
			}
			AddActionTile(srcX, srcY, x * scaledSize, y * scaledSize, isSolid, isAnimated , isWinning, isMysteryBox);
			isSolid = false;
			isAnimated = false;
			isWinning = false;
			isMysteryBox = false;
			x++;
		}
		x = 0;
		y++;
		if (y == 20)
			break;
	}

	mapFile.close();
	x = 0;
	y = 0;

	mapFile.open(foregroundpath);

	while (getline(mapFile, line)) ////read foreground layer
	{
		std::stringstream str(line);

		while (getline(str, word, ',')) //this is searching in foreground
		{
			wordNum = stoi(word);
			srcY = (wordNum / 26) * tileSize;
			srcX = (wordNum % 26) * tileSize; //adding tile based on srcX,srcY coordinates

			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_FOREGROUND_TILES; arrayTilesIndex++)
			{
				if (wordNum == foregroundTiles[arrayTilesIndex])
				{
					AddForegroundTile(srcX, srcY, x * scaledSize, y * scaledSize, isSolid);
				}
			}
			x++;
		}
		x = 0;
		y++;
		if (y == 20)
			break;
	}
	mapFile.close();

	x = 0;
	y = 0;

	mapFile.open(backgroundlayerpath);

	while (getline(mapFile, line)) ////read background
	{
		std::stringstream str(line);

		while (getline(str, word, ',')) //this is searching in background
		{
			wordNum = stoi(word);
			srcY = (wordNum / 26) * tileSize;
			srcX = (wordNum % 26) * tileSize; //adding tile based on srcX,srcY coordinates

			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_BACKGROUND_TILES; arrayTilesIndex++)
			{
				if (wordNum == backgroundTiles[arrayTilesIndex])
				{
					AddBackgroundTile(srcX, srcY, x * scaledSize, y * scaledSize, isSolid);
				}
			}
			x++;
		}
		x = 0;
		y++;
		if (y == 20)
			break;
	}
	
	mapFile.close();
	
	x = 0;
	y = 0;

	mapFile.open(sewerbackgroundlayerpath);

	while (getline(mapFile, line)) ////read background
	{
		std::stringstream str(line);

		while (getline(str, word, ',')) //this is searching in background
		{
			wordNum = stoi(word);
			srcY = (wordNum / 26) * tileSize;
			srcX = (wordNum % 26) * tileSize; //adding tile based on srcX,srcY coordinates

			for (arrayTilesIndex = 0; arrayTilesIndex < NUM_SEWER_BACKGROUND_TILES; arrayTilesIndex++)
			{
				if (wordNum == sewerbackgroundTiles[arrayTilesIndex])
				{
					AddSewersBackgroundTile(srcX, srcY, 3040 + x * scaledSize, y * scaledSize, isSolid);
				}
			}
			x++;
		}
		x = 0;
		y++;
		if (y == 20)
			break;
	}

	mapFile.close();

	x = 0;
	y = 0;
	
}

void Map::AddActionTile(int srcX, int srcY, int xpos, int ypos, bool isSolid, bool isAnimated, bool isWinning, bool isMysteryBox)
{
	auto& tile(manager.addEntity());
	
	if (isWinning)
	{
		tile.addGroup(Game::groupWinningTiles);
	}
	else
	{
		tile.addGroup(Game::groupActionLayer);
	}

	if (isSolid)
	{
		tile.addGroup(Game::groupColliders);
	}

	if (isAnimated)
	{
		tile.addComponent<TileComponent>(-32, -32, xpos, ypos, tileSize, mapScale, texID, isSolid, true, false); //insert tile and grid and colliders(somehow we refer to background)
		tile.getComponent<TransformComponent>().position.x = xpos;
		tile.getComponent<TransformComponent>().position.y = ypos;
		tile.addComponent<AnimatorComponent>(texID);
		tile.addComponent<MovingAnimatorComponent>(texID);
		tile.addComponent<PlatformBlock_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
		if (srcY)
		{
			tile.getComponent<AnimatorComponent>().Play("DarkBlock");
		}
		else
		{
			tile.getComponent<AnimatorComponent>().Play("BrownBlock");
		}
		
	}
	else
	{
		tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, true, false); //insert tile and grid and colliders(somehow we refer to background)
		if (isMysteryBox)
		{
			tile.getComponent<TransformComponent>().position.x = xpos;
			tile.getComponent<TransformComponent>().position.y = ypos;
			tile.addComponent<AnimatorComponent>(texID);
			tile.addComponent<MovingAnimatorComponent>(texID);
			tile.addComponent<MysteryBox_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
			tile.getComponent<AnimatorComponent>().Play("QuestionMark");
		}
	}
}

void Map::AddForegroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false, false); //insert foregroundtile
	tile.addGroup(Game::groupForegroundLayer);
}

void Map::AddBackgroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false, true); //insert backgroundtile
	tile.addGroup(Game::groupBackgroundLayer);
}

void Map::AddSewersBackgroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false, false); //insert backgroundtile
	tile.addGroup(Game::groupSewerBackgroundLayer);
}