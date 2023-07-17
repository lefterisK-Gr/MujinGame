#pragma once

#include "../Components.h"

class GreenKoopaTroopa_Script : public Component
{
public:
	bool shelltoturtle = false;
	AnimatorComponent* animator;
	SpriteComponent* sprite;

	GreenKoopaTroopa_Script()
	{

	}

	~GreenKoopaTroopa_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		int timeslice = 0;

		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height - 20;
		sprite->srcRect.h = sprite->transform->height + 20;
		
		sprite->destRect.y = static_cast<int>(sprite->transform->position.y - 20) - Game::camera.y; //make player move with the camera, being stable in centre, except on edges
		sprite->destRect.h = (sprite->transform->height * sprite->transform->scale) + 20;

		if (shelltoturtle)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			if (Game::justResumed)//if we just resumed
			{
				animator->resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - animator->resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 2));
			if (timeslice == 1)// on finish
			{
				sprite->initTime = 0;
				shelltoturtle = false;
				animator->Play("GreenKoopaTroopaWalk");
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->transform->velocity.x = -1;
			}
		}

	}
};