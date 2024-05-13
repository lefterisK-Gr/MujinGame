#ifndef GAME_H
#define GAME_H

#include <GameScreen/IGameScreen.h>
#include <SDL/SDL.h>
#include <SDL_IMAGE/SDL_image.h>
#include <GL/glew.h>
#include "GLSLProgram.h"
#undef main
#include <iostream>
#include <vector>
#include "Camera2D/Camera2D.h"
#include "SpriteBatch/SpriteBatch.h"
#include "InputManager/InputManager.h"
#include "Timing/Timing.h"
//#include "SpriteFont/SpriteFont.h"
#include "AudioEngine/AudioEngine.h"
#include "Window/Window.h"
#include "TextureManager/TextureManager.h"

#include "ECS/ECS.h"

#include "GameScreen/ScreenIndices.h"
#include "Vector2D/Vector2D.h"


class Map;
class AssetManager;
class SceneManager;
class ColliderComponent;
class TransformComponent;

struct Cell {
	std::vector<Entity*> entities;
};

class Grid {
public:
	Grid(int width, int height, int cellSize) :
		_width(width),
		_height(height),
		_cellSize(cellSize) {
		_numXCells = ceil((float)_width / _cellSize);
		_numYCells = ceil((float)_height / _cellSize);

		//allocate all the cells
		_cells.resize(_numXCells * _numYCells);
	}
	~Grid() {

	}

	void addEntity(Entity* entity) {
		//Cell* cell = getCell(entity->GetComponent<TransformComponent>().position);
		//cell->entities.push_back(entity);

	}

	Cell* getCell(int x, int y) {
		if (x < 0) x = 0;
		if (x > _numXCells) x = _numXCells - 1;
		if (y < 0) y = 0;
		if (y > _numYCells) x = _numYCells - 1;

		return &_cells[y * _numXCells + x];
	}
	Cell* getCell(const Vector2D& pos) {
		int cellX = (int)(pos.x / _cellSize);
		int cellY = (int)(pos.y / _cellSize);

		return getCell(cellX, cellY);
	}

private:
	std::vector<Cell> _cells; //cache friendly
	int _cellSize;
	int _width;
	int _height;
	int _numXCells;
	int _numYCells;
};

class Game : public IGameScreen {

public:
	Game(MujinEngine::Window* window);
	~Game();


	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;

	virtual void destroy() override;

	virtual void onEntry() override;

	virtual void onExit() override;

	virtual void update(float deltaTime) override;

	virtual void draw() override;

	/////////////////////////
	
	void setupShaderAndLightTexture(const std::string& textureName, Camera2D& camera);
	void setupShaderAndTexture(const std::string& textureName, Camera2D& camera);
	void renderBatch(const std::vector<Entity*>& entities, SpriteBatch& batch);
	void drawHUD(const std::vector<Entity*>& entities, const std::string& textureName);

	/////////////////////////

	static SDL_Event event;

	static Camera2D camera2D;
	static Camera2D hudCamera2D;

	static SpriteBatch _spriteBatch;
	static SpriteBatch _hudSpriteBatch;

	static AudioEngine audioEngine;

	static Map* map;

	static AssetManager* assets;

	static SceneManager* scenes;

	//std::unique_ptr<Grid> grid;

	enum groupLabels : std::size_t //todo should add groups at end for some reason
	{
		groupBackgroundLayer,
		groupSewerBackgroundLayer,
		groupActionLayer,
		groupPlayers,
		groupBackgrounds,
		//groupEnemies,
		groupColliders,
		groupMysteryBoxes,
		groupWinningTiles,
		groupSlices,
		groupEnemySlices,
		groupLights,
		groupTextureLights,
		groupProjectiles,
		groupWarriorProjectiles,
		groupSkeletons,
		groupLabels,
		groupStageUpButtons,
		groupStageUpAttackButtons,
		groupStageUpDefenceButtons,
		groupStageUpHpButtons,
		groupStageUpButtonsBack,
		groupSlots,
		groupShops,
		groupInventories,
		groupGreenKoopaTroopas,
		groupPipeRingForeground,
		groupForegroundLayer,
		groupMarket,
		screenShapes,
		groupHPBars
	};

	static MujinEngine::Window* _window;

private:
	void checkInput();
	bool onPauseGame();

	GLSLProgram _colorProgram;
	GLSLProgram _textureLightProgram;
	GLSLProgram _textureProgram;
	GLSLProgram _lightProgram;

	SDL_Rect _mouseCoords = { -100, -100 , 1, 1 };
	//SpriteFont* _spriteFont;

	int _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	int _prevScreenIndex = SCREEN_INDEX_GAMEPLAY;

	const float SCALE_SPEED = 0.1f;
};


#endif