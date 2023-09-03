#include "Map.h"
//#include "TextureManager.h"

extern Manager manager;

int solidTiles[] = {52,17,5,0 ,208 , 6 , 130 , 131 , 132 , 75, 78, 79 , 80 , 14, 15 , 40 , 41, 64 , 65 , 90 , 91 , 522 , 521 , 548, 524, 496, 470 , 495 ,144 };
int foregroundTiles[] = { 32,54,55,56,58 , 33 , 7, 59 ,161 , 162, 163 ,34, 57 , 9, 35, 14, 15 , 40 , 41 };
int backgroundTiles[] = { 446,447,448,472,473,474,498,499,500 };
int sewerbackgroundTiles[] = { 549 };
int bouncyTiles[] = { 52 , 144 };
int mysteryBoxTiles[] = { 208 };
int winningTiles[] = { 496, 470 , 495 };

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

bool Map::tileHasFeature(Entity& tile, int wordNum, int featureTileArray[]) {
	int arrayTilesIndex = 0;

	for (arrayTilesIndex = 0; arrayTilesIndex < sizeof(featureTileArray) / sizeof(featureTileArray[0]); arrayTilesIndex++)
	{
		if (wordNum == featureTileArray[arrayTilesIndex])
		{
			return true;//(this->*addTileFeature)(tile);
		}
	}
	return false;
}

void Map::addBouncyTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, bouncyTiles)) {
		tile.addComponent<MovingAnimatorComponent>(texID);
		tile.addComponent<PlatformBlock_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
	}
}

void Map::addWinningTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, winningTiles)) {
		tile.addGroup(Game::groupWinningTiles);
	}
}

void Map::addMysteryBoxTileFeature(Entity& tile, int wordNum) {
	if (tileHasFeature(tile, wordNum, mysteryBoxTiles)) {
		auto& tileComp = tile.getComponent<TileComponent>();
		auto& tile2(manager.addEntity());
		tile2.addComponent<TileComponent>(tileComp.srcRect.x, tileComp.srcRect.y, tileComp.position.x, tileComp.position.y, tileSize, mapScale, texID, tileComp.fullSolid, false); //insert tile and grid and colliders(somehow we refer to background)
		tile2.addComponent<AnimatorComponent>(texID);
		tile2.addComponent<MovingAnimatorComponent>(texID);
		tile2.addComponent<MysteryBox_Script>(); //insert tile and grid (texID is set in Game::init() ("terrain"))
		tile2.getComponent<AnimatorComponent>().Play("QuestionMark");
		tile2.addGroup(Game::groupForegroundLayer);
		tile2.addGroup(Game::groupMysteryBoxes);
	}
}

void Map::LoadMap(std::string backgroundlayerpath, std::string sewerbackgroundlayerpath, std::string actionlayerpath, std::string foregroundpath)
{
	std::fstream mapFile;

	TileFeatureCallback addBouncyFeature = &Map::addBouncyTileFeature;
	TileFeatureCallback addWinningFeature = &Map::addWinningTileFeature;
	TileFeatureCallback addMysteryBoxFeature = &Map::addMysteryBoxTileFeature;

	tileFeatures.push_back(addBouncyFeature);
	tileFeatures.push_back(addWinningFeature);
	tileFeatures.push_back(addMysteryBoxFeature);

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