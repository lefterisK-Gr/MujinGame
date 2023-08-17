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

int solidTiles[] = { 3,52,5,0 ,208 , 6 , 130 , 131 , 132 , 75, 78, 79 , 80 , 14, 15 , 40 , 41, 64 , 65 , 90 , 91 , 522 , 521 , 548, 524, 496, 470 , 495 ,338 };
int foregroundTiles[] = { 32,54,55,56,58 , 33 , 7, 59 ,10 , 11, 12 ,34, 57 , 9, 35, 14, 15 , 40 , 41 };
int backgroundTiles[] = { 446,447,448,472,473,474,498,499,500 };
int sewerbackgroundTiles[] = { 549 };
int animatedTiles[] = { 3 , 338 };
int mysteryBoxTiles[] = { 208 };
int winningTiles[] = { 496, 470 , 495 };

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) //probably initiallization
{
	scaledSize = ms * ts;
}

Map::~Map()
{

}

void Map::ProcessLayer(std::fstream& mapFile, int tileArray[], void (Map::* addTileFunction)(int, int, int, int, bool)) {
	
	bool isSolid = false;
	int x = 0, y = 0;

	int wordNum = 0;
	int arrayTilesIndex = 0;

	int srcX, srcY;

	std::string line, word;

	while (getline(mapFile, line)) //reading tiles (action layer)
	{
		std::stringstream str(line);

		while (getline(str, word, ',')) //this is searching in tilemap
		{
			wordNum = stoi(word);
			srcY = (wordNum / 26) * tileSize;
			srcX = (wordNum % 26) * tileSize; //adding tile based on srcX,srcY coordinates

			for (arrayTilesIndex = 0; arrayTilesIndex < (sizeof(tileArray) / sizeof(tileArray[0])); arrayTilesIndex++) {
				if (wordNum == tileArray[arrayTilesIndex]) {
					isSolid = true;
					break;
				}
			}

			(this->*addTileFunction)(srcX, srcY, x * scaledSize, y * scaledSize, isSolid);
			isSolid = false;
			x++;
		}
		x = 0;
		y++;
		if (y == 20) {
			break;
		}
	}
	mapFile.close();
}

void Map::LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath, std::string actionlayerpath, std::string foregroundpath)
{
	bool isSolid = false;
	bool isAnimated = false;
	bool isWinning = false;
	bool isMysteryBox = false;
	
	int x = 0, y = 0;
	int wordNum = 0;
	int arrayTilesIndex = 0;

	int srcX, srcY;

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
			
			for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(solidTiles) / sizeof(solidTiles[0]); arrayTilesIndex++)
			{
				if (wordNum == solidTiles[arrayTilesIndex])
				{
					isSolid = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(mysteryBoxTiles) / sizeof(mysteryBoxTiles[0]); arrayTilesIndex++)
			{
				if (wordNum == mysteryBoxTiles[arrayTilesIndex])
				{
					isMysteryBox = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(animatedTiles) / sizeof(animatedTiles[0]); arrayTilesIndex++)
			{
				if (wordNum == animatedTiles[arrayTilesIndex])
				{
					isAnimated = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(winningTiles) / sizeof(winningTiles[0]); arrayTilesIndex++)
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
	ProcessLayer(mapFile, foregroundTiles, &Map::AddForegroundTile);
	mapFile.close();

	mapFile.open(backgroundlayerpath);
	ProcessLayer(mapFile, backgroundTiles, &Map::AddBackgroundTile);
	mapFile.close();

	mapFile.open(sewerbackgroundlayerpath);
	ProcessLayer(mapFile, sewerbackgroundTiles, &Map::AddSewersBackgroundTile);
	mapFile.close();
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
	tile.addComponent<TileComponent>(srcX, srcY, xpos + 3040, ypos, tileSize, mapScale, texID, isSolid, false, false); //insert backgroundtile
	tile.addGroup(Game::groupSewerBackgroundLayer);
}