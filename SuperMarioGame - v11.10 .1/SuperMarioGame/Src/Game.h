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
#include "Camera2D.h"
#include "SpriteBatch/SpriteBatch.h"
#include "InputManager/InputManager.h"
#include "Timing/Timing.h"
#include "SpriteFont/SpriteFont.h"
#include "AudioEngine/AudioEngine.h"
#include "Window/Window.h"

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
	
	void setupShaderAndTexture(const std::string& textureName);
	void renderBatch(const std::vector<Entity*>& entities);
	void drawHUD(const std::string& textureName);

	/////////////////////////

	static SDL_Event event;

	static SDL_Rect camera;
	static Camera2D camera2D;
	static Camera2D hudCamera2D;

	static SpriteBatch _spriteBatch;
	static SpriteBatch _hudSpriteBatch;

	static AudioEngine audioEngine;

	static Map* map;

	static AssetManager* assets;

	static SceneManager* scenes;

	enum groupLabels : std::size_t //todo should add groups at end for some reason
	{
		groupBackgroundLayer,
		groupSewerBackgroundLayer,
		groupActionLayer,
		groupPlayers,
		//groupEnemies,
		groupColliders,
		groupMysteryBoxes,
		groupWinningTiles,
		groupSlices,
		groupEnemySlices,
		groupLights,
		groupProjectiles,
		groupSkeletons,
		groupGreenKoopaTroopas,
		groupPipeRingForeground,
		groupForegroundLayer,
		screenShapes,
		groupHPBars
	};

private:
	void checkInput();
	bool onPauseGame();

	MujinEngine::Window* _window;
	GLSLProgram _colorProgram;
	GLSLProgram _textureProgram;
	GLSLProgram _lightProgram;

	SpriteFont* _spriteFont;

	int _nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	int _prevScreenIndex = SCREEN_INDEX_GAMEPLAY;

	const float SCALE_SPEED = 0.1f;
};


#endif