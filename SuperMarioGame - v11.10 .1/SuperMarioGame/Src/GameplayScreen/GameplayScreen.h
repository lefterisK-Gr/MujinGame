#pragma once

#include "../GameScreen/IGameScreen.h"
#include <SDL/SDL.h>
#include <SDL_IMAGE/SDL_image.h>
#include <GL/glew.h>
#include "../GLSLProgram.h"
#undef main
#include <iostream>
#include <vector>
#include "../Camera2D.h"
#include "../SpriteBatch/SpriteBatch.h"
#include "../InputManager/InputManager.h"
#include "../Timing/Timing.h"
#include "../SpriteFont/SpriteFont.h"
#include "../AudioEngine/AudioEngine.h"
#include "../Window/Window.h"

#include "../ECS/ECS.h"
#include "../TextureManager.h"
#include "../Map/Map.h"
#include "../ECS/Components.h"
#include "../Vector2D.h"
#include "../Collision/Collision.h"
#include "../AssetManager/AssetManager.h"
#include "../SceneManager.h"

class GameplayScreen : public IGameScreen {
public:
	GameplayScreen();
	~GameplayScreen();

    virtual int getNextScreenIndex() const override;

    virtual int getPreviousScreenIndex() const override;

    virtual void build() override;

    virtual void destroy() override;

    virtual void onEntry() override;

    virtual void onExit() override;

    virtual void update() override;

    virtual void draw() override;

    static SDL_Rect camera;
    static Camera2D camera2D;
    static Camera2D hudCamera2D;

    static SpriteBatch _spriteBatch;
    static SpriteBatch _hudSpriteBatch;

    static InputManager _inputManager;
    static AudioEngine audioEngine;


    static AssetManager* assets;

    static SceneManager* scenes;
private:
    void checkInput();

    MujinEngine::Window _window;
    GLSLProgram _colorProgram;

    SpriteFont* _spriteFont;
};