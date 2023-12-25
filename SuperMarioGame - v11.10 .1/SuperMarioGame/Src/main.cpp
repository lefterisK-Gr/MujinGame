
#include "Game.h"

Game* game = nullptr;


//please don't put all your code in main like I did.
int main(int argc, const char* argv[]) {

	const float DESIRED_FPS = 60;
	const int MAX_PHYSICS_STEPS = 6;

	game = new Game();
	game->init("Mujin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false, 60.0f);

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float prevTicks = SDL_GetTicks();

	while (game->running())
	{
		game->_fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		game->handleEvents();
		if (!game->paused())
		{
			int i = 0;
			while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
			{
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
				game->update(deltaTime);
				totalDeltaTime -= deltaTime;
				i++;
				game->render();
			}
		}
		
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
