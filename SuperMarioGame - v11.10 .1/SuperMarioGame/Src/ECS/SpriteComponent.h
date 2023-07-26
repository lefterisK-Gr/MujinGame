#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"
// TODO: (extra): can add states for different states (0 for full solid tile or 1 for no solid
class SpriteComponent : public Component //sprite -> transform
{
private:
	SDL_Texture *texture;
public:
	int initTime = 0; // time that starts the animation
	TransformComponent* transform;
	SDL_Rect srcRect, destRect;
	Vector2D tempPosition;
	bool isPipe = false;

	int animIndexX = 0; //index in line of spritesheet
	int animIndexY = 0; //index in column of spritesheet
	int total_frames = 1; // number of differentt sprites for animation
	float speed = 1; //speed of exchanging sprites

	Animation animation;
	
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, Vector2D pos, bool isP)
	{
		setTex(id);
		tempPosition = pos;
		isPipe = isP;
	}

	~SpriteComponent()
	{
		//SDL_DestroyTexture(texture); //no need for that anymore, because sprite points to a texture that could be used by multiple objects
	}

	void setTex(std::string id) //this function is used to change texture of a sprite
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
		
		if (isPipe)
		{
			transform->position = tempPosition;
		}
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height; 
	}

	void update() override
	{
		if (isPipe)
		{
			destRect.x = transform->position.x - Game::camera.x; //reminding that destRect is absolutely on the screen that we see
			destRect.y = transform->position.y - Game::camera.y;

			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;
		}
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void SetAnimation(int idX, int idY, int fr, float sp, const Animation::animType type)
	{
		if (isPipe)
		{
			srcRect.x = animIndexX * srcRect.w;
			srcRect.y = animIndexY * srcRect.h;
		}

		animIndexX = idX;
		animIndexY = idY;
		
		total_frames = fr;
		speed = sp;

		animation = Animation(idX, idY, fr, sp, type);
	}

	void setFrame() {
		this->srcRect.x = (this->animation.indexX * this->transform->width) /* init */ + ( this->srcRect.w * animation.cur_frame_index/* curframe from total frams */);
		this->srcRect.y = this->animation.indexY * this->transform->height;
	}

	void DestroyTex()
	{
		//TextureManager::DestroyTexture(texture);
		texture = NULL;
	}
};