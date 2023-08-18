#pragma once
//#include "Game.h"
#include <string>
#include "Game.h"
#include <fstream>
#include <sstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"
class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void ProcessLayer(std::fstream& mapFile, int tileArray[], void (Map::* addTileFunction)(Entity&, int, int, int, int, bool));
	void LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath,std::string actionlayerpath, std::string foregroundpath);
	void AddActionTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid, bool isBouncy, bool isWinning, bool isMysteryBox);
	void AddForegroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddSewersBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	//void DrawMap();
private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
};
