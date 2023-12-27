#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include "../GameScreen/IMainGame.h"

GameplayScreen::GameplayScreen(){}
GameplayScreen::~GameplayScreen(){}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build() {

}

void GameplayScreen::destroy() {

}

void GameplayScreen::onEntry() {
	std::cout << "onEntry" << std::endl;
}

void GameplayScreen::onExit() {

}

void GameplayScreen::update() {
	std::cout << "update" << std::endl;
	checkInput();
}

void GameplayScreen::draw() {
	std::cout << "draw" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

}

void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		_game->onSDLEvent(evnt);
	}
}