#include "IMainGame.h"
#include "../Timing/Timing.h"

#include "ScreenList.h"
#include "IGameScreen.h"

IMainGame::IMainGame() {
	_screenList = std::make_unique<ScreenList>(this);
}

IMainGame::~IMainGame() {

}

void IMainGame::run() {

	if (!init()) return;

	FPSLimiter limiter;
	limiter.setMaxFPS(60.0f);

	_isRunning = true;
	while (_isRunning) {
		limiter.begin();

		_inputManager.update();
		update();
		draw();
		
		_fps = limiter.end();
		_window.swapBuffer();
	}
}
void IMainGame::exitGame() {
	_currentScreen->onExit();
	if (_screenList) {
		_screenList->destroy();
		_screenList.reset();
	}
	_isRunning = false;
}

void IMainGame::onSDLEvent(SDL_Event& evnt) {
	switch (evnt.type)
	{
	case SDL_QUIT:
		exitGame();
		break;
	case SDL_KEYDOWN:
		_inputManager.pressKey(evnt.key.keysym.sym);
		break;
	case SDL_KEYUP:
		_inputManager.releaseKey(evnt.key.keysym.sym);
	case SDL_MOUSEMOTION:
		//std::cout << event.motion.x << " " << event.motion.y << std::endl;
		_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
		break;
	//case SDL_MOUSEWHEEL:
	//	if (evnt.wheel.y > 0)
	//	{
	//		// Scrolling up
	//		camera2D.setScale(camera2D.getScale() + SCALE_SPEED);
	//	}
	//	else if (evnt.wheel.y < 0)
	//	{
	//		// Scrolling down
	//		camera2D.setScale(camera2D.getScale() - SCALE_SPEED);
	//	}
	//	break;
	case SDL_MOUSEBUTTONDOWN:
		_inputManager.pressKey(evnt.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		_inputManager.releaseKey(evnt.button.button);
		break;
	default:
		break;
	}
	if (_inputManager.isKeyDown(SDLK_ESCAPE)) {
		exitGame();
	}

	/*if (_inputManager.isKeyPressed(SDLK_p)) {
		if (Game::isPaused)
		{
			Game::justResumed = true;
			Game::isPaused = false;
		}
		else
		{
			Game::isPaused = true;
			Game::pauseTime = SDL_GetTicks();
		}
	}*/

	/*if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = camera2D.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
	}*/

	_inputManager.update();
}

bool IMainGame::init() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Tell SDL that we want a double buffered window so we dont get
	//any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	
	if (!initSystems()) return false;

	onInit();
	addScreens();

	_currentScreen = _screenList->getCurrent();
	_currentScreen->onEntry();
	_currentScreen->setRunning();

	return true;
}

bool IMainGame::initSystems() {
	_window.create("Mujin", 800, 640, 0);
	return true;
}

void IMainGame::update() {
	if (_currentScreen) {
		switch (_currentScreen->getState()) {
		case ScreenState::RUNNING:
			_currentScreen->update();
			break;
		case ScreenState::CHANGE_NEXT:
			_currentScreen->onExit();
			_currentScreen = _screenList->moveNext();
			if (_currentScreen) {
				_currentScreen->setRunning();
				_currentScreen->onEntry();
			}
			break;
		case ScreenState::CHANGE_PREVIOUS:
			_currentScreen->onExit();
			_currentScreen = _screenList->movePrevious();
			if (_currentScreen) {
				_currentScreen->setRunning();
				_currentScreen->onEntry();
			}
			break;
		case ScreenState::EXIT_APPLICATION:
			exitGame();
			break;
		default:
			break;
		}
	}
	else {
		exitGame();
	}

}
void IMainGame::draw() {
	glViewport(0, 0, _window.getScreenWidth(), _window.getScreenHeight());
	if (_currentScreen && _currentScreen->getState() == ScreenState::RUNNING) {
		_currentScreen->draw();
	}
}