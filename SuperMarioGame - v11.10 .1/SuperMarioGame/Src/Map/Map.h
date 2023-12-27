#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "../ECS\ECS.h"
#include "../ECS\Components.h"

#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0])) 

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void ProcessLayer(std::fstream& mapFile, void (Map::* addTileFunction)(Entity&, int, int, int, int, bool));
	void LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath,std::string actionlayerpath, std::string foregroundpath);
	void AddActionTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddForegroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddSewersBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);

	void addBouncyTileFeature(Entity& tile, int wordNum);
	void addWinningTileFeature(Entity& tile, int wordNum);
	void addMysteryBoxTileFeature(Entity& tile, int wordNum);
	void addPipeTileFeature(Entity& tile, int wordNum);
	bool tileHasFeature(Entity& tile, int wordNum, int featureTileArray[], int featureTileArraySize); //, void (Map::* addTileFeature)(Entity&)
	//void DrawMap();
private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
};

using TileFeatureCallback = void (Map::*)(Entity& tile, int wordNum);
