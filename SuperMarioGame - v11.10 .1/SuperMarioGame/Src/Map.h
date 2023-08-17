#pragma once
//#include "Game.h"
#include <string>

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void ProcessLayer(std::fstream& mapFile, int tileArray[], void (Map::* addTileFunction)(int, int, int, int, bool));
	void LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath,std::string actionlayerpath, std::string foregroundpath);
	void AddActionTile(int srcX, int srcY, int xpos, int ypos, bool isSolid, bool isAnimated, bool isWinning, bool isMysteryBox);
	void AddForegroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddBackgroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddSewersBackgroundTile(int srcX, int srcY, int xpos, int ypos, bool isSolid);
	//void DrawMap();
private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
};
