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

class AssetManager;
class SceneManager;
class ColliderComponent;

class Game : public IGameScreen {

public:
	Game();
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
	void drawHUD();

	/////////////////////////

	static SDL_Event event;

	static uint32_t pauseTime;

	static bool isRunning;
	static bool isPaused;
	static bool justResumed;

	static SDL_Rect camera;
	static Camera2D camera2D;
	static Camera2D hudCamera2D;

	static SpriteBatch _spriteBatch;
	static SpriteBatch _hudSpriteBatch;

	float _fps;

	static InputManager _inputManager;
	static AudioEngine audioEngine;


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
		groupProjectiles,
		groupSkeletons,
		groupGreenKoopaTroopas,
		groupPipeRingForeground,
		groupForegroundLayer,
		screenShapes
	};

private:
	void checkInput();

	MujinEngine::Window _window;
	GLSLProgram _colorProgram;

	SpriteFont* _spriteFont;

	const float SCALE_SPEED = 0.1f;
};


#endif