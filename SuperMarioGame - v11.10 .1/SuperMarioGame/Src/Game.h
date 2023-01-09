#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#undef main
#include <iostream>
#include <vector>

class AssetManager;
class SceneManager;
class ColliderComponent;

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int x, int ypos, int width, int height, bool fullscreen);

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

	static AssetManager* assets;

	static SceneManager* scenes;

	enum groupLabels : std::size_t
	{
		groupBackgroundLayer,
		groupSewerBackgroundLayer,
		groupActionLayer,
		groupPlayers,
		//groupEnemies,
		groupColliders,
		groupWinningTiles,
		groupProjectiles,
		groupGoombas,
		groupGreenKoopaTroopas,
		groupPipeRingForeground,
		groupForegroundLayer
	};

private:
	int cnt = 0;
	SDL_Window* window;
};


#endif