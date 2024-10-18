#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "ECS\ECSManager.h"

#include <algorithm>
#include <random>
#include <ctime>

#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0])) 

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	std::vector<std::vector<int>> generateRandomMap(int width, int height);
	std::vector<std::vector<int>> getRandomTileSet();
	void generateRandomBackgroundMap(const std::string& inputMapPath, const std::string& outputMapPath);

	void saveMapToCSV(const std::vector<std::vector<int>>& map, const std::string& fileName);
	void ProcessLayer(std::fstream& mapFile, void (Map::* addTileFunction)(Entity&, int, int, int, int, bool));
	glm::ivec2 GetLayerDimensions(std::string mapFile);
	void LoadMap(std::string background1layerpath, std::string background2layerpath, std::string actionlayerpath, std::string foregroundpath);
	void AddActionTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddForeActionTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);
	void AddBackActionTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid);

	void addBouncyTileFeature(Entity& tile, int wordNum);
	void addWinningTileFeature(Entity& tile, int wordNum);
	void addMysteryBoxTileFeature(Entity& tile, int wordNum);
	void addMarketTileFeature(Entity& tile, int wordNum);
	void addPipeTileFeature(Entity& tile, int wordNum);
	bool tileHasFeature(Entity& tile, int wordNum, int featureTileArray[], int featureTileArraySize); //, void (Map::* addTileFeature)(Entity&)
	//void DrawMap();

	void setMapCompleted(bool isCompleted);
	bool getMapCompleted();

	void resetMap();

	int getStage();
private:
	int stage = 0;
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;

	bool isMapCompleted = false;
};

using TileFeatureCallback = void (Map::*)(Entity& tile, int wordNum);
