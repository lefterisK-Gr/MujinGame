#ifndef GAME_H
#define GAME_H

#include "GameScreen/IGameScreen.h"
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

class Map;
class AssetManager;
class SceneManager;
class ColliderComponent;

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
	
	void setupShaderAndTexture(const std::string& textureName, Camera2D& camera);
	void renderBatch(const std::vector<Entity*>& entities, SpriteBatch& batch);
	void drawHUD(const std::vector<Entity*>& entities, const std::string& textureName);

	/////////////////////////

	static SDL_Event event;

	static SDL_Rect camera;
	static Camera2D camera2D;
	static Camera2D hudCamera2D;

	static SpriteBatch _spriteBatch;
	static SpriteBatch _hudSpriteBatch;

	static AudioEngine audioEngine;

	static Map* map;

	static TextureManager* textures;
	static AssetManager* assets;

	static SceneManager* scenes;

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
	GLSLProgram _textureProgram;
	GLSLProgram _lightProgram;

	SDL_Rect _mouseCoords = { -100, -100 , 1, 1 };
	//SpriteFont* _spriteFont;

	int _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	int _prevScreenIndex = SCREEN_INDEX_GAMEPLAY;

	const float SCALE_SPEED = 0.1f;
};


#endif