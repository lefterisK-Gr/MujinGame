#include "MainMenuScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include "ECS/Components.h"
#include "Camera2D/CameraManager.h"
#include "../ECS/ScriptComponents.h"
#include "Vector2D/Vector2D.h"
#include "../Collision/Collision.h"
#include "../AssetManager/AssetManager.h"
#include "GameScreen/IMainGame.h"

Manager main_menu_manager;

Collision main_menu_collision;

SpriteBatch MainMenuScreen::_spriteBatch;

AssetManager* MainMenuScreen::assets = nullptr;

auto& StartGameButton(main_menu_manager.addEntity());
auto& ExitGameButton(main_menu_manager.addEntity());

MainMenuScreen::MainMenuScreen(MujinEngine::Window* window)
	: _window(window)
{
	_screenIndex = SCREEN_INDEX_MAIN_MENU;
}

MainMenuScreen::~MainMenuScreen()
{
	//dtor
}

int MainMenuScreen::getNextScreenIndex() const
{
	return _nextScreenIndex;
}

int MainMenuScreen::getPreviousScreenIndex() const
{
	return _prevScreenIndex;
}

void MainMenuScreen::build()
{

}

void MainMenuScreen::destroy()
{

}

void MainMenuScreen::onEntry()
{
	assets = new AssetManager(&manager, _game->_inputManager, _game->_window);

	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("mainMenu_hud"));

	hud_camera2D->init(_window->getScreenWidth(), _window->getScreenHeight()); // Assuming a screen resolution of 800x600
	hud_camera2D->setPosition(hud_camera2D->getPosition() /*+ glm::vec2(
		width / 2.0f,
		height / 2.0f
	)*/);;
	hud_camera2D->setScale(1.0f);

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//InitShaders function from Bengine
		_textureProgram.compileShaders("Src/Shaders/textureBright.vert", "Src/Shaders/textureBright.frag");
		_textureProgram.addAttribute("vertexPosition");
		_textureProgram.addAttribute("vertexColor");
		_textureProgram.addAttribute("vertexUV");
		_textureProgram.linkShaders();

		MainMenuScreen::_spriteBatch.init();
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	TextureManager::getInstance().Add_GLTexture("startgame", "assets/Sprites/StartGame.png");
	TextureManager::getInstance().Add_GLTexture("exitgame", "assets/Sprites/ExitGame.png");

	StartGameButton.addComponent<TransformComponent>(230.0f, 100.0f,
		TextureManager::getInstance().Get_GLTexture("startgame")->height,
		TextureManager::getInstance().Get_GLTexture("startgame")->width,
		1.0f);
	StartGameButton.addComponent<SpriteComponent>("startgame", 1.0f, true);
	StartGameButton.addComponent<ButtonComponent>(std::bind(&MainMenuScreen::onStartGame, this));
	StartGameButton.addGroup(MainMenuScreen::startGameGroup);

	ExitGameButton.addComponent<TransformComponent>(240.0f, 250.0f,
		TextureManager::getInstance().Get_GLTexture("exitgame")->height,
		TextureManager::getInstance().Get_GLTexture("exitgame")->width,
		1.0f);
	ExitGameButton.addComponent<SpriteComponent>("exitgame", 1.0f, true);
	ExitGameButton.addComponent<ButtonComponent>(std::bind(&MainMenuScreen::onExitGame, this));
	ExitGameButton.addGroup(MainMenuScreen::exitGameGroup);
}
auto& startgamebuttons(main_menu_manager.getGroup(MainMenuScreen::startGameGroup));
auto& exitgamebuttons(main_menu_manager.getGroup(MainMenuScreen::exitGameGroup));
void MainMenuScreen::onExit()
{
	for (auto& sb : startgamebuttons)
	{
		sb->GetComponent<ButtonComponent>().setOnClick(std::bind(&MainMenuScreen::onResumeGame, this));
	}
}



void MainMenuScreen::update(float deltaTime)
{
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("mainMenu_hud"));

	checkInput();

	main_menu_manager.refresh();
	main_menu_manager.update(deltaTime);

	hud_camera2D->update();


	for (auto& sb : startgamebuttons)
	{
		SpriteComponent entitySprite = sb->GetComponent<SpriteComponent>();
		if (main_menu_collision.checkCollision(entitySprite.destRect, _mouseCoords)) { //culling
			std::cout << "clicked button" << std::endl;
			sb->GetComponent<ButtonComponent>().setState(ButtonComponent::ButtonState::PRESSED);
		}
	}
	for (auto& eb : exitgamebuttons)
	{
		SpriteComponent entitySprite = eb->GetComponent<SpriteComponent>();
		if (main_menu_collision.checkCollision(entitySprite.destRect, _mouseCoords)) { //culling
			std::cout << "clicked button" << std::endl;
			eb->GetComponent<ButtonComponent>().setState(ButtonComponent::ButtonState::PRESSED);
		}
	}

	_mouseCoords.x = -100.0f;
	_mouseCoords.y = -100.0f;

}

void MainMenuScreen::setupShaderAndTexture(const std::string& textureName) {
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("mainMenu_hud"));

	_textureProgram.use();
	glActiveTexture(GL_TEXTURE0);
	const GLTexture* texture = TextureManager::getInstance().Get_GLTexture(textureName);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint textureLocation = _textureProgram.getUniformLocation("texture_sampler");
	glUniform1i(textureLocation, 0);
	GLint pLocation = _textureProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = hud_camera2D->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

void MainMenuScreen::renderBatch(const std::vector<Entity*>& entities) {
	_spriteBatch.begin();
	for (const auto& entity : entities) {
		entity->draw(_spriteBatch, *Game::_window);
	}
	_spriteBatch.end();
	_spriteBatch.renderBatch();
}

void MainMenuScreen::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	//////////////////////////////////////

	setupShaderAndTexture("startgame");
	renderBatch(startgamebuttons);
	setupShaderAndTexture("exitgame");
	renderBatch(exitgamebuttons);

	glBindTexture(GL_TEXTURE_2D, 0);
	//drawHUD();
	_textureProgram.unuse();
}

void MainMenuScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		
		_game->onSDLEvent(evnt);

		if (_game->_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			glm::vec2 mouseCoordsVec = _game->_inputManager.getMouseCoords();
			_mouseCoords.x = mouseCoordsVec.x;
			_mouseCoords.y = mouseCoordsVec.y;
			std::cout << _mouseCoords.x << " " << _mouseCoords.y << std::endl;
		}

		_game->_inputManager.update();

	}
}

bool MainMenuScreen::onStartGame() {
	_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	_currentState = ScreenState::CHANGE_NEXT;
	return true;
}

bool MainMenuScreen::onResumeGame() {
	_prevScreenIndex = SCREEN_INDEX_GAMEPLAY;
	_currentState = ScreenState::CHANGE_PREVIOUS;
	return true;
}

void MainMenuScreen::onExitGame() {
	_game->exitGame();
}