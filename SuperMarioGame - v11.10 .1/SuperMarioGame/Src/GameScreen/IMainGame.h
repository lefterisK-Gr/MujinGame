#pragma once

#include "../Game.h"
#include "../Window/Window.h"
#include <memory>

class ScreenList;
class IGameScreen;

class IMainGame {
public:
	IMainGame();
	virtual ~IMainGame();

	void run();
	void exitGame();

	virtual void onInit() = 0;
	virtual void addScreens() = 0;
	virtual void onExit() = 0;

	const float getFps() const {
		return _fps;
	}

protected:
	virtual void update();
	virtual void draw();

	void onSDLEvent(SDL_Event& evnt);

	bool init();
	bool initSystems();

	std::unique_ptr<ScreenList> _screenList = nullptr;
	IGameScreen* _currentScreen = nullptr;
	bool _isRunning = false;
	float _fps = 0.0f;
	MujinEngine::Window _window;
	InputManager _inputManager;

	const float SCALE_SPEED = 0.1f;
};