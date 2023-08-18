#include "Map.h"
//#include "TextureManager.h"


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
int bouncyTiles[] = { 3 , 338 };
int mysteryBoxTiles[] = { 208 };
int winningTiles[] = { 496, 470 , 495 };

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) //probably initiallization
{
	scaledSize = ms * ts;
}

Map::~Map()
{

}

void Map::ProcessLayer(std::fstream& mapFile, int tileArray[], void (Map::* addTileFunction)(Entity&, int, int, int, int, bool)) {
	
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

			auto& tile(manager.addEntity());

			for (arrayTilesIndex = 0; arrayTilesIndex < (sizeof(tileArray) / sizeof(tileArray[0])); arrayTilesIndex++) {
				if (wordNum == tileArray[arrayTilesIndex]) {
					isSolid = true;
					break;
				}
			}

			(this->*addTileFunction)(tile, srcX, srcY, x * scaledSize, y * scaledSize, isSolid);
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
	bool isBouncy = false;
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
			for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(bouncyTiles) / sizeof(bouncyTiles[0]); arrayTilesIndex++)
			{
				if (wordNum == bouncyTiles[arrayTilesIndex])
				{
					isBouncy = true;
				}
			}
			for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(winningTiles) / sizeof(winningTiles[0]); arrayTilesIndex++)
			{
				if (wordNum == winningTiles[arrayTilesIndex])
				{
					isWinning = true;
				}
			}
			auto& tile(manager.addEntity());
			AddActionTile(tile, srcX, srcY, x * scaledSize, y * scaledSize, isSolid, isBouncy, isWinning, isMysteryBox);
			isSolid = false;
			isBouncy = false;
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

void Map::AddActionTile(Entity &tile, int srcX, int srcY, int xpos, int ypos, bool isSolid, bool isBouncy, bool isWinning, bool isMysteryBox)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, true, false); //insert tile and grid and colliders(somehow we refer to background)

	if (isBouncy)
	{
		tile.addComponent<MovingAnimatorComponent>(texID);
		tile.addComponent<PlatformBlock_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
	}

	if(isMysteryBox)
	{
		/*auto& tile2(manager.addEntity());
		tile2.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, true, false);*/ //insert tile and grid and colliders(somehow we refer to background)
		//tile2.addComponent<AnimatorComponent>(texID);
		//tile2.addComponent<MovingAnimatorComponent>(texID);
		//tile2.addComponent<MysteryBox_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
		//tile2.getComponent<AnimatorComponent>().Play("QuestionMark");
		/*tile2.addGroup(Game::groupActionLayer);
		tile2.addGroup(Game::groupColliders);*/
	}

	tile.addGroup(Game::groupActionLayer);

	if (isWinning)
	{
		tile.addGroup(Game::groupWinningTiles);
	}

	if (isSolid)
	{
		tile.addGroup(Game::groupColliders);
	}
}

void Map::AddForegroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false, false); //insert foregroundtile
	tile.addGroup(Game::groupForegroundLayer);
}

void Map::AddBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false, true); //insert backgroundtile
	tile.addGroup(Game::groupBackgroundLayer);
}

void Map::AddSewersBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos + 3040, ypos, tileSize, mapScale, texID, isSolid, false, false); //insert backgroundtile
	tile.addGroup(Game::groupSewerBackgroundLayer);
}