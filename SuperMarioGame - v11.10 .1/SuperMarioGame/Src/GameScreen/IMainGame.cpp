#include "IMainGame.h"
#include "../Timing.h"

#include "ScreenList.h"
#include "IGameScreen.h"

IMainGame::IMainGame() {

}

IMainGame::~IMainGame() {

}

void IMainGame::run() {

	if (!init() == false) return;

	FPSLimiter limiter;
	limiter.setMaxFPS(60.0f);

	_isRunning = true;
	while (_isRunning) {
		limiter.begin();

		_inputManager.update();
		update();
		draw();
		
		_fps = limiter.end();
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
		_isRunning = false;
		break;
	case SDL_KEYDOWN:
		Game::_inputManager.pressKey(evnt.key.keysym.sym);
		break;
	case SDL_KEYUP:
		Game::_inputManager.releaseKey(evnt.key.keysym.sym);
	case SDL_MOUSEMOTION:
		//std::cout << event.motion.x << " " << event.motion.y << std::endl;
		_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
		break;
	case SDL_MOUSEWHEEL:
		if (evnt.wheel.y > 0)
		{
			// Scrolling up
			Game::camera2D.setScale(Game::camera2D.getScale() + SCALE_SPEED);
		}
		else if (evnt.wheel.y < 0)
		{
			// Scrolling down
			Game::camera2D.setScale(Game::camera2D.getScale() - SCALE_SPEED);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		Game::_inputManager.pressKey(evnt.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		Game::_inputManager.releaseKey(evnt.button.button);
		break;
	default:
		break;
	}
}

bool IMainGame::init() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Tell SDL that we want a double buffered window so we dont get
	//any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	
	if (!initSystems()) return false;

	initSystems();

	onInit();
	addScreens();

	_currentScreen = _screenList->getCurrent();
	_currentScreen->onEntry();
	_currentScreen->setRunning();

	return true;
}

bool IMainGame::initSystems() {
	_window.create("Default", 800, 640, 0);
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