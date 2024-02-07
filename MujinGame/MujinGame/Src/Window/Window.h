#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace MujinEngine {

    enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

    class Window
    {
    public:
        Window();
        ~Window();

        int create(std::string windowName, int screenWidth, int screenHeight, float scale, unsigned int currentFlags);

        void swapBuffer();

        int getScreenWidth() { return _screenWidth; }
        int getScreenHeight() { return _screenHeight; }
        float getScale() { return _scale; }
    private:
        SDL_Window* _sdlWindow;
        int _screenWidth, _screenHeight;
        float _scale;
    };

}