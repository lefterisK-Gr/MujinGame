#include "TextureManager.h"

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
	//SDL_SetTextureColorMod(tex, 100, 100, 100);
	//SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MOD);
	//SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
	
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}
