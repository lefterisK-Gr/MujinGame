
#include "Game.h"

Game* game = nullptr;


//please don't put all your code in main like I did.
int main(int argc, const char* argv[]) {

	game = new Game();
	game->init("Mujin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false, 60.0f);

	while (game->running())
	{
		game->_fpsLimiter.begin();

		game->handleEvents();
		if (!game->paused())
		{
			game->update();
		}
		game->render();
		game->justResumed = false;

		game->_fps = game->_fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << game->_fps << std::endl;
			frameCounter = 0;
		}
	}
	game->clean();

	return 0;
}
