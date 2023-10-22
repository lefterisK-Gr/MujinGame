#include "TextureManager.h"
#include <fstream>
#include "picoPNG.h"
#include "Errors.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::DestroyTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest , SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}

bool TextureManager::readFileToBuffer(std::string filePath, std::vector <unsigned char>& buffer) {
	std::ifstream file(filePath, std::ios::binary);

	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//seek to the end
	file.seekg(0, std::ios::end);

	//Get the file size
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//Reduce the file size by any header bytes that might be present
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);
	file.close();

	return true;
}

//GLTexture* TextureManager::loadPNG(std::string filePath) {
//	GLTexture texture = {};
//
//	std::vector<unsigned char> in;
//	std::vector<unsigned char> out;
//
//	unsigned long width, height;
//	if (TextureManager::readFileToBuffer(filePath, in) == false) {
//		fatalError("Failed to load PNG file to buffer!");
//	}
//
//	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
//
//	if (errorCode != 0) {
//		fatalError("decodePNG failed with error: " + std::to_string(errorCode));
//	}
//
//	glGenTextures(1, &(texture.id));
//
//	glBindTexture(GL_TEXTURE_2D, texture.id);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	texture.width = width;
//	texture.height = height;
//
//	return &texture;
//}
