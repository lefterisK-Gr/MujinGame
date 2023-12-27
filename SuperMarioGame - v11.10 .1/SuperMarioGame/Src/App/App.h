#pragma once

#include "../GameScreen/IMainGame.h"
#include "../GameplayScreen/GameplayScreen.h"
#include "../Game.h"

class App : public IMainGame
{
public:
	App();
	~App();

    // Called on initialization
    virtual void onInit() override;
    // For adding all screens
    virtual void addScreens() override;
    // Called when exiting
    virtual void onExit() override;
private:
    std::unique_ptr<Game> _gameplayScreen = nullptr;
    //std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
    //std::unique_ptr<EditorScreen> m_editorScreen = nullptr;
};

