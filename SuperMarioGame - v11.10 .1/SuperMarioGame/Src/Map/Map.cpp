#include "Map.h"
//#include "TextureManager.h"

extern Manager manager;

int solidTiles[] = {52,69,177 ,176 , 112 , 16 , 17 , 18 , 75, 0, 1 , 2 , 48, 49 , 50 , 51, 83 , 85 , 99 , 101 , 522 , 521 , 548, 524, 496, 470 , 495 ,68 };
int foregroundTiles[] = { 32,54,55,56,58 , 33 , 7, 59 ,161 , 162, 163 ,34, 57 , 115, 131, 48, 49 , 50 , 51 };
int backgroundTiles[] = { 178, 179, 208, 209, 210, 224, 225, 226 };
int sewerbackgroundTiles[] = { 549 };
int bouncyTiles[] = { 52 };
int mysteryBoxTiles[] = { 176 };
int winningTiles[] = { 495, 470, 496 };
int pipeTiles[] = { 48, 49 , 50 , 51 , 83 , 85 , 99 , 101, 35, 36 };

std::vector<TileFeatureCallback> tileFeatures;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) //probably initiallization
{
	scaledSize = ms * ts;
}

Map::~Map()
{

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

void Map::LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath, std::string actionlayerpath, std::string foregroundpath)
{
	std::fstream mapFile;

	TileFeatureCallback addBouncyFeature = &Map::addBouncyTileFeature;
	TileFeatureCallback addWinningFeature = &Map::addWinningTileFeature;
	TileFeatureCallback addMysteryBoxFeature = &Map::addMysteryBoxTileFeature;
	TileFeatureCallback addPipeFeature = &Map::addPipeTileFeature;

	tileFeatures.push_back(addBouncyFeature);
	tileFeatures.push_back(addWinningFeature);
	tileFeatures.push_back(addMysteryBoxFeature);
	tileFeatures.push_back(addPipeFeature);

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

	mapFile.open(sewerbackgroundlayerpath);
	ProcessLayer(mapFile, &Map::AddSewersBackgroundTile);
	mapFile.close();
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