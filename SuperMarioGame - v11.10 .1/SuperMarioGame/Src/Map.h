#pragma once
//#include "Game.h"
#include <string>

#define NUM_SOLID_TILES 29
#define NUM_FOREGROUND_TILES 15
#define NUM_BACKGROUND_TILES 9
#define NUM_ANIMATED_TILES 2
#define NUM_MYSTERY_BOX_TILES 1
#define NUM_WINNING_TILES 3
#define NUM_SEWER_BACKGROUND_TILES 1

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath,std::string actionlayerpath, std::string foregroundpath);
	void AddActionTile(int srcX, int srcY, int xpos, int ypos, bool isSolid, bool isAnimated, bool isWinning, bool isMysteryBox);
	void AddForegroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddBackgroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddSewersBackgroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid);
	//void DrawMap();
private:
	int solidTiles[NUM_SOLID_TILES] = {3,52,5,0 ,208 , 6 , 130 , 131 , 132 , 75, 78, 79 , 80 , 14, 15 , 40 , 41, 64 , 65 , 90 , 91 , 522 , 521 , 548, 524, 496, 470 , 495 ,338 };
	int foregroundTiles[NUM_FOREGROUND_TILES] = { 32,54,55,56,58 , 33 , 7, 59 ,10 , 11, 12 ,34, 57 , 9, 35 };
	int backgroundTiles[NUM_BACKGROUND_TILES] = { 446,447,448,472,473,474,498,499,500};
	int sewerbackgroundTiles[NUM_SEWER_BACKGROUND_TILES] = { 549 };
	int animatedTiles[NUM_ANIMATED_TILES] = { 3 , 338 };
	int mysteryBoxTiles[NUM_MYSTERY_BOX_TILES] = { 208 };
	int winningTiles[NUM_WINNING_TILES] = { 496, 470 , 495 };
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
};
