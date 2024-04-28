#include "Window.h"
#include "../ConsoleLogger.h"


MujinEngine::Window::Window()
{
}


MujinEngine::Window::~Window()
{
}

int MujinEngine::Window::create(std::string windowName, int screenWidth, int screenHeight, float scale, unsigned int currentFlags) {

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    _screenWidth = screenWidth * scale;
    _screenHeight = screenHeight * scale;
    _scale = scale;

    if (currentFlags & INVISIBLE) {
        flags |= SDL_WINDOW_HIDDEN;
    }
    if (currentFlags & VISIBLE) {
        flags &= ~INVISIBLE;
    }
    if (currentFlags & FULLSCREEN) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (currentFlags & BORDERLESS) {
        flags |= SDL_WINDOW_BORDERLESS;
    }

    //Open an SDL window
    _sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);
    if (_sdlWindow == nullptr) {
        MujinEngine::ConsoleLogger::error("SDL Window could not be created!");
    }

    //Set up our OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
    if (glContext == nullptr) {
        MujinEngine::ConsoleLogger::error("SDL_GL context could not be created!");
    }

    //Set up glew (optional but recommended)
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        MujinEngine::ConsoleLogger::error("Could not initialize glew!");
    }

    //Check the OpenGL version
    std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

    //Set VSYNC
    SDL_GL_SetSwapInterval(0);

    // Enable alpha blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void MujinEngine::Window::swapBuffer() {
    SDL_GL_SwapWindow(_sdlWindow);
}

