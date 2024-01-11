#include "Map.h"
#include "../ECS/Components.h"


extern Manager manager;

int solidTiles[] = {52,69,177 ,176 , 112 , 16 , 17 , 18 , 75, 0, 1 , 2 , 48, 49 , 50 , 51, 83 , 85 , 99 , 101 , 522 , 521 , 548, 524, 496, 470 , 495 ,68 };
int foregroundTiles[] = { 32,54,55,56,58 , 33 , 7, 59 ,161 , 162, 163 ,34, 57 , 115, 131, 48, 49 , 50 , 51 };
int backgroundTiles[] = { 178, 179, 208, 209, 210, 224, 225, 226 };
//int sewerbackgroundTiles[] = { 549 };
int bouncyTiles[] = { 52 };
int mysteryBoxTiles[] = { 176 };
int winningTiles[] = { 495, 470, 496 };
int pipeTiles[] = { 48, 49 , 50 , 51 , 83 , 85 , 99 , 101, 35, 36 };

std::vector<TileFeatureCallback> tileFeatures;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) //probably initiallization
{
	scaledSize = ms * ts;

	TileFeatureCallback addBouncyFeature = &Map::addBouncyTileFeature;
	TileFeatureCallback addWinningFeature = &Map::addWinningTileFeature;
	TileFeatureCallback addMysteryBoxFeature = &Map::addMysteryBoxTileFeature;
	TileFeatureCallback addPipeFeature = &Map::addPipeTileFeature;

	tileFeatures.push_back(addBouncyFeature);
	tileFeatures.push_back(addWinningFeature);
	tileFeatures.push_back(addMysteryBoxFeature);
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
		map[0][x] = 69;          // Top row
		map[height - 1][x] = 69; // Bottom row
	}

	// Set 69 on the left and right columns
	for (int y = 1; y < height - 1; ++y) {
		map[y][0] = 69;          // Left column
		map[y][width - 1] = 69;  // Right column
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

	std::uniform_int_distribution<int> distTopRow(maxWidthGap, maxWidthGap + 3); // For top row
	std::uniform_int_distribution<int> distTopRowHeight(-2, 2); // For other rows, excluding top and bottom
	
	int lastTopRow= distTopRow(rng);
	int lastTopHeight = distTopRowHeight(rng);

	while (lastTopRow < width - maxWidthGap) {
		int startTopRow = lastTopRow + 3; // Next platform position in width
		int startTopHeight = distTopRowHeight(rng); // Random platform position in height

		// Ensure the platform height is within the map bounds
		startTopHeight = max(2, min(lastTopHeight + startTopHeight, height - maxHeightGap));

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
		tile.addGroup(Game::groupWinningTiles);
	}
}

void Map::addMysteryBoxTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, mysteryBoxTiles, ARRAY_SIZE(mysteryBoxTiles))) {
		auto& tileComp = tile.getComponent<TileComponent>();
		auto& tile2(manager.addEntity());
		tile2.addComponent<TileComponent>(tileComp.srcRect.x, tileComp.srcRect.y, tileComp.position.x, tileComp.position.y, tileSize, mapScale, texID, tileComp.fullSolid, false); //insert tile and grid and colliders(somehow we refer to background)
		tile2.addComponent<AnimatorComponent>(texID);
		tile2.addComponent<MovingAnimatorComponent>(texID);
		tile2.addComponent<MysteryBox_Script>(Game::audioEngine.loadSoundEffect("Sounds/coin_collect.wav")); //insert tile and grid (texID is set in Game::init() ("terrain"))
		tile2.getComponent<AnimatorComponent>().Play("QuestionMark");
		tile2.addGroup(Game::groupForegroundLayer);
		tile2.addGroup(Game::groupMysteryBoxes);
	}
}

void Map::addPipeTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, pipeTiles, ARRAY_SIZE(pipeTiles))) {
		tile.addGroup(Game::groupForegroundLayer);
	}
}

void Map::LoadMap(std::string backgroundlayerpath, std::string actionlayerpath, std::string foregroundpath)
{
	std::fstream mapFile;

	mapFile.open(actionlayerpath);
	ProcessLayer(mapFile, &Map::AddActionTile);
	mapFile.close();

	tileFeatures.clear();

	mapFile.open(foregroundpath);
	ProcessLayer(mapFile, &Map::AddForegroundTile);
	mapFile.close();

	mapFile.open(backgroundlayerpath);
	ProcessLayer(mapFile, &Map::AddBackgroundTile);
	mapFile.close();

	/*mapFile.open(sewerbackgroundlayerpath);
	ProcessLayer(mapFile, &Map::AddSewersBackgroundTile);
	mapFile.close();*/
}

void Map::AddActionTile(Entity &tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false); //insert tile and grid and colliders(somehow we refer to background)

	tile.addGroup(Game::groupActionLayer);

	if (isSolid)
	{
		tile.addGroup(Game::groupColliders);
	}
}

void Map::AddForegroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, false); //insert foregroundtile
	tile.addGroup(Game::groupForegroundLayer);
}

void Map::AddBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID, isSolid, true); //insert backgroundtile
	tile.addGroup(Game::groupBackgroundLayer);
}

void Map::AddSewersBackgroundTile(Entity& tile, int srcX, int srcY, int xpos, int ypos, bool isSolid)
{
	tile.addComponent<TileComponent>(srcX, srcY, xpos + 3040, ypos, tileSize, mapScale, texID, isSolid, false); //insert backgroundtile
	tile.addGroup(Game::groupSewerBackgroundLayer);
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

	saveMapToCSV(generateRandomMap(120, 20), "assets/RandomMap.csv");

	LoadMap("assets/background_v3.csv", "assets/RandomMap.csv", "");
}

int Map::getStage() {
	return stage;
}