#include "Map.h"
#include "ECS/Components.h"
#include "../ECS/ScriptComponents.h"

extern Manager manager;

int solidTiles[] = {52,69,177 ,176 , 112 , 16 , 17 , 18 , 75, 0, 1 , 2 , 48, 49 , 50 , 51, 83 , 85 , 99 , 101 , 522 , 521 , 548, 524, 496, 470 , 495 ,68 };
int bouncyTiles[] = { 52 };
int mysteryBoxTiles[] = { 176 };
int winningTiles[] = { 495, 470, 496 };
int marketTiles[] = { 217,228, 249,218,229,250,218,229,250 };
int pipeTiles[] = { 48, 49 , 50 , 51 , 83 , 85 , 99 , 101, 35, 36 };

std::vector<TileFeatureCallback> tileFeatures;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) //probably initiallization
{
	scaledSize = ms * ts;

	TileFeatureCallback addBouncyFeature = &Map::addBouncyTileFeature;
	TileFeatureCallback addWinningFeature = &Map::addWinningTileFeature;
	TileFeatureCallback addMysteryBoxFeature = &Map::addMysteryBoxTileFeature;
	TileFeatureCallback addMarketFeature = &Map::addMarketTileFeature;
	TileFeatureCallback addPipeFeature = &Map::addPipeTileFeature;

	tileFeatures.push_back(addBouncyFeature);
	tileFeatures.push_back(addWinningFeature);
	tileFeatures.push_back(addMysteryBoxFeature);
	tileFeatures.push_back(addMarketFeature);
	tileFeatures.push_back(addPipeFeature);
}

Map::~Map()
{

}

std::vector<std::vector<int>> Map::generateRandomMap(int width, int height) {
	std::vector<std::vector<int>> map(height, std::vector<int>(width, -1));
	std::vector<int> topRowPlatform = { 0, 1, 2 };
	std::vector<int> bodyPlatform = { 16, 17, 18 };

	// Set 69 on the top and bottom rows
	for (int x = 0; x < width; ++x) {
		map[height - 1][x] = 1; // Bottom row
	}


	if (width >= 2 && height >= 2) {
		//top left pipe
		map[0][2] = 50;
		map[0][3] = 51;
		map[1][2] = 35;
		map[1][3] = 36;

		//bottom right pipe
		map[height - 3][width - 2] = 83;
		map[height - 3][width - 1] = 85;
		map[height - 2][width - 2] = 99;
		map[height - 2][width - 1] = 101;
	}

	// Initialize random number generation
	std::mt19937 rng(time(nullptr));

	// Define the max width and height for a platform
	const int maxWidthGap = 10; // Maximum gap allowed in width
	const int maxHeightGap = 5; // Maximum gap allowed in height

	std::uniform_int_distribution<int>	distTopRow(maxWidthGap, maxWidthGap + 3); // For top row
	std::normal_distribution<float>		distTopRowHeight(0.0, 1.0); // For other rows, excluding top and bottom
	
	int lastTopRow= distTopRow(rng);
	int lastTopHeight = 2;

	while (lastTopRow < width - maxWidthGap) {
		int startTopRow = lastTopRow + 3; // Next platform position in width
		int heightVariation = static_cast<int>(std::round(distTopRowHeight(rng)));
		heightVariation = max(-2, min(heightVariation, 2)); // Clamp between -2 and 2

		int startTopHeight = max(2, min(lastTopHeight + heightVariation, height - maxHeightGap));

		map[height - startTopHeight][startTopRow] = 0;
		map[height - startTopHeight][startTopRow + 1] = 1;
		map[height - startTopHeight][startTopRow + 2] = 2;

		// Extend body platform down to the floor
		for (int y = height - startTopHeight + 1; y < height - 1; ++y) {  // Start from 1 to avoid overwriting the top row
			map[y][startTopRow] = 16;
			map[y][startTopRow + 1] = 17;
			map[y][startTopRow + 2] = 18;
		}

		// Update last position for next iteration
		lastTopRow = startTopRow;
		lastTopHeight = startTopHeight;
	}

	return map;
}

std::vector<std::vector<int>> Map::getRandomTileSet() {
	std::vector<std::vector<std::vector<int>>> tileSets = {
		{{133}},
		{{40, 41, 42},{24,25,26} },
		{{-1}},
		{{-1}},
		{{-1}},
		{{-1}},
		{{14,15}},
		{{131},{115}},
		{{161,162,163}},
		{{244,245,246},{228,229,230},{212,213,214}},
		{{-1,254,-1},{237,238,239},{221,222,223},{205,206,207},{189,190,191}}

		// Add more sets as needed
	};

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, tileSets.size() - 1);

	return tileSets[dist(rng)];
}

void Map::generateRandomBackgroundMap(const std::string& inputMapPath, const std::string& outputMapPath) {
	std::ifstream inputFile(inputMapPath);
	std::ofstream outputFile(outputMapPath);
	std::string line;

	if (!inputFile.is_open() || !outputFile.is_open()) {
		std::cerr << "Error opening files." << std::endl;
		return;
	}

	std::vector<std::vector<int>> mapData, backgroundMap;

	// Read and store the main map data
	while (getline(inputFile, line)) {
		std::stringstream ss(line);
		std::string tile;
		std::vector<int> row;

		while (getline(ss, tile, ',')) {
			row.push_back(stoi(tile));
		}

		mapData.push_back(row);
		backgroundMap.push_back(std::vector<int>(row.size(), -1));
	}

	// Process the map data and set the foreground tiles
	for (size_t y = 0; y < mapData.size(); ++y) {
		for (size_t x = 0; x < mapData[y].size(); ++x) {
			if ((mapData[y][x] == 0 && mapData[y][x+1] == 1 && mapData[y][x+2] == 2) && y > 0) {
				std::vector<std::vector<int>> tileSet = getRandomTileSet();

				for (size_t ty = 0; ty < tileSet.size() && (y - ty - 1) < backgroundMap.size(); ++ty) {
					for (size_t tx = 0; tx < tileSet[ty].size() && (x + tx) < backgroundMap[y].size(); ++tx) {
						backgroundMap[y - ty - 1][x + tx] = tileSet[ty][tx];
					}
				}
			}
		}
	}

	if (backgroundMap.size() >= 19 && backgroundMap[0].size() >= 1) {
		backgroundMap[16][1] = 169; // Tile at x=1, y=16
		backgroundMap[17][1] = 185; // Tile at x=1, y=17
		backgroundMap[18][1] = 201; // Tile at x=1, y=18
	}

	if (backgroundMap.size() >= 19 && backgroundMap[0].size() >= 1) {
		backgroundMap[16][3] = 217;
		backgroundMap[17][3] = 233;
		backgroundMap[18][3] = 249;

		backgroundMap[16][4] = 218;
		backgroundMap[17][4] = 234;
		backgroundMap[18][4] = 250;

		backgroundMap[16][5] = 219;
		backgroundMap[17][5] = 235;
		backgroundMap[18][5] = 251;

		//backgroundMap[16][6] = 21;
		//backgroundMap[17][6] = 229;
		//backgroundMap[18][6] = 250;
	}

	// Write the foreground map to the output CSV file
	for (const auto& row : backgroundMap) {
		for (size_t i = 0; i < row.size(); ++i) {
			outputFile << row[i];
			if (i < row.size() - 1) {
				outputFile << ",";
			}
		}
		outputFile << "\n";
	}

	inputFile.close();
	outputFile.close();

}

void Map::saveMapToCSV(const std::vector<std::vector<int>>& map, const std::string& fileName) {
	std::ofstream file(fileName);

	if (!file.is_open()) {
		std::cerr << "Failed to open file for writing: " << fileName << std::endl;
		return;
	}

	for (const auto& row : map) {
		for (size_t i = 0; i < row.size(); ++i) {
			file << row[i];
			if (i < row.size() - 1)
				file << ",";
		}
		file << "\n";
	}

	file.close();
}

void Map::ProcessLayer(std::fstream& mapFile, void (Map::* addTileFunction)(Entity&, int, int, int, int, bool)) {
	
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

			if (wordNum >= 0) {
				srcY = (wordNum / 16) * tileSize;
				srcX = (wordNum % 16) * tileSize; //adding tile based on srcX,srcY coordinates

				for (arrayTilesIndex = 0; arrayTilesIndex < (sizeof(solidTiles) / sizeof(solidTiles[0])); arrayTilesIndex++)
				{
					if (wordNum == solidTiles[arrayTilesIndex])
					{
						isSolid = true;
						break;
					}
				}

				auto& tile(manager.addEntity());
				(this->*addTileFunction)(tile, srcX, srcY, x * scaledSize, y * scaledSize, isSolid);

				for (int i = 0; i < tileFeatures.size(); i++) {
					(this->*tileFeatures[i])(tile, wordNum);
				}
			}
			

			isSolid = false;
			x++;
		}
		x = 0;
		y++;
		if (y == 20) {
			break;
		}
	}
}

SDL_Rect Map::GetLayerDimensions(std::string filePath)
{
	std::fstream mapFile;

	mapFile.open(filePath);

	if (!mapFile.is_open()) {
		throw std::runtime_error("File stream is not open or in a bad state.");
	}

	std::string line;
	int width = 0, height = 0;

	if (getline(mapFile, line)) {
		std::stringstream ss(line);
		std::string cell;

		while (getline(ss, cell, ',')) {
			width++; 
		}

		height++;  
	}

	while (getline(mapFile, line)) {
		height++;  
	}

	mapFile.clear();  
	mapFile.seekg(0);

	return SDL_Rect{ 0, 0, width * scaledSize, height * scaledSize };
}

bool Map::tileHasFeature(Entity& tile, int wordNum, int featureTileArray[], int featureTileArraySize) {
	int arrayTilesIndex = 0;

	for (arrayTilesIndex = 0; arrayTilesIndex < featureTileArraySize; arrayTilesIndex++)
	{
		if (wordNum == featureTileArray[arrayTilesIndex])
		{
			return true;//(this->*addTileFeature)(tile);
		}
	}
	return false;
}

void Map::addBouncyTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, bouncyTiles, ARRAY_SIZE(bouncyTiles))) {
		tile.addComponent<MovingAnimatorComponent>(texID);
		tile.addComponent<PlatformBlock_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
	}
}

void Map::addWinningTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, winningTiles, ARRAY_SIZE(winningTiles))) {
		tile.addGroup(Manager::groupWinningTiles);
	}
}

void Map::addMysteryBoxTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, mysteryBoxTiles, ARRAY_SIZE(mysteryBoxTiles))) {
		auto& tileComp = tile.GetComponent<TileComponent>();
		auto& tile2(manager.addEntity());
		tile2.addComponent<TileComponent>(tileComp.srcRect.x, tileComp.srcRect.y, tileComp.position.x, tileComp.position.y, tileSize, mapScale, texID, tileComp.fullSolid, false); //insert tile and grid and colliders(somehow we refer to background)
		tile2.addComponent<AnimatorComponent>(texID);
		tile2.addComponent<MovingAnimatorComponent>(texID);
		tile2.addComponent<MysteryBox_Script>(Game::audioEngine.loadSoundEffect("Sounds/coin_collect.wav")); //insert tile and grid (texID is set in Game::init() ("terrain"))
		tile2.GetComponent<AnimatorComponent>().Play("QuestionMark");
		tile2.addGroup(Manager::groupForegroundLayer);
		tile2.addGroup(Manager::groupMysteryBoxes);
	}
}

void Map::addMarketTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, marketTiles, ARRAY_SIZE(marketTiles))) {
		tile.addComponent<ColliderComponent>("market");
		tile.addGroup(Manager::groupMarket);
	}
}

void Map::addPipeTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, pipeTiles, ARRAY_SIZE(pipeTiles))) {
		tile.addGroup(Manager::groupForegroundLayer);
	}
}

void Map::LoadMap(std::string background1layerpath, std::string background2layerpath, std::string actionlayerpath, std::string foregroundpath)
{
	std::fstream mapFile;

	mapFile.open(actionlayerpath);
	ProcessLayer(mapFile, &Map::AddActionTile);
	mapFile.close();

	mapFile.open(foregroundpath);
	ProcessLayer(mapFile, &Map::AddForegroundTile);
	mapFile.close();

	mapFile.open(background1layerpath);
	ProcessLayer(mapFile, &Map::AddSewersBackgroundTile);
	mapFile.close();

	mapFile.open(background2layerpath);
	ProcessLayer(mapFile, &Map::AddBackgroundTile);
	mapFile.close();
}

void Map::AddActionTile(Entity &tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false); //insert tile and grid and colliders(somehow we refer to background)

	tile.addGroup(Manager::groupActionLayer);

	if (isSolid)
	{
		tile.addGroup(Manager::groupColliders);
	}
}

void Map::AddForegroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false); //insert foregroundtile
	tile.addGroup(Manager::groupForegroundLayer);
}

void Map::AddBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, true); //insert backgroundtile
	tile.addGroup(Manager::groupBackgroundLayer);
}

void Map::AddSewersBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false); //insert backgroundtile
	tile.addGroup(Manager::groupSewerBackgroundLayer);
}

void Map::setMapCompleted(bool completed)
{
	isMapCompleted = completed;
}

bool Map::getMapCompleted()
{
	return isMapCompleted;
}

void Map::resetMap()
{
	setMapCompleted(false);

	stage++;

	saveMapToCSV(generateRandomMap(120, 20), "assets/Maps/RandomMap.csv");

	generateRandomBackgroundMap("assets/Maps/RandomMap.csv", "assets/Maps/RandomBackground.csv");

	LoadMap("assets/Maps/RandomBackground.csv","assets/Maps/background_v3.csv", "assets/Maps/RandomMap.csv", "");
}

int Map::getStage() {
	return stage;
}