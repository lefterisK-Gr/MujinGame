#pragma once

#include <vector>
#include "GLTexture.h"
#include <string>

class TextureManager {

public: 
	//OPENGL functions
	static bool readFileToBuffer(const char* filePath, std::vector <unsigned char>& buffer);
	static GLTexture* loadPNG(const char* filePath);
};