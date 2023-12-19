#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL_IMAGE/SDL_image.h>
#include <GL/glew.h>
#include "GLSLProgram.h"
#undef main
#include <iostream>
#include <vector>
#include "Camera2D.h"
#include "SpriteBatch.h"
#include "InputManager.h"
#include "Timing.h"

class AssetManager;
class SceneManager;
class ColliderComponent;

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int x, int ypos, int width, int height, bool fullscreen, float _maxFPS);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }
	bool paused() { return isPaused; }

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static uint32_t pauseTime;

	static bool isRunning;
	static bool isPaused;
	static bool justResumed;

	static SDL_Rect camera;
	static Camera2D camera2D;

	static SpriteBatch _spriteBatch;

	FPSLimiter _fpsLimiter;
	float _fps;

	static InputManager _inputManager;

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
		groupProjectiles,
		groupGoombas,
		groupGreenKoopaTroopas,
		groupPipeRingForeground,
		groupForegroundLayer,
		screenShapes
	};

private:
	SDL_Window* window;
	SDL_GLContext gOpenGLContext;
	GLSLProgram _colorProgram;

	const float SCALE_SPEED = 0.1f;

	float _time = 0.0f;
};


#endif