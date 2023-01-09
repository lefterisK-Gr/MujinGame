
#include "Game.h"

Game* game = nullptr;


//please don't put all your code in main like I did.
int main(int argc, const char* argv[]) {

	const int FPS = 60; //frames per second
	const int frameDelay = 1000 / FPS; //time for 60 frames per second

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("Mujin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	while (game->running())
	{

		frameStart = SDL_GetTicks();

		game->handleEvents();
		if (!game->paused())
		{
			game->update();
		}
		game->render();
		game->justResumed = false;

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();

	return 0;
}
