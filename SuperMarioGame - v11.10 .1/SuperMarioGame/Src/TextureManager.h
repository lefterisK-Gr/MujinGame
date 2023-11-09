#pragma once

#include "Game.h"
#include <vector>
#include "GLTexture.h"
#include <string>

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static void DestroyTexture(SDL_Texture* texture);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip); 

	//OPENGL functions
	static bool readFileToBuffer(const char* filePath, std::vector <unsigned char>& buffer);
	static GLTexture* loadPNG(const char* filePath);
};