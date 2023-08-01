#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include "MovingAnimation.h"
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

	Animation animation;
	MovingAnimation moving_animation;
	
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, Vector2D pos)
	{
		setTex(id);
		tempPosition = pos;
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
		
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height; 
	}

	void update() override
	{

	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void SetAnimation(int idX, int idY, int fr, float sp, const Animation::animType type)
	{
		animation = Animation(idX, idY, fr, sp, type);
	}

	void SetMovingAnimation(int idX, int idY, int fr, float sp, const Animation::animType type, int dx, int dy)
	{
		moving_animation = MovingAnimation(idX, idY, fr, sp, type, dx, dy);
	}

	void setCurrFrame() {
		this->srcRect.x = (this->animation.indexX * this->transform->width) /* init */ + ( this->srcRect.w * animation.cur_frame_index/* curframe from total frams */);
		this->srcRect.y = this->animation.indexY * this->transform->height;
	}

	void DestroyTex()
	{
		//TextureManager::DestroyTexture(texture);
		texture = NULL;
	}
};