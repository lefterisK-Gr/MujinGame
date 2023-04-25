#pragma once

#include "../AnimatorComponent.h"

class GreenKoopaTroopa_AnimatorComponent : public AnimatorComponent
{
public:
	bool shelltoturtle = false;

	GreenKoopaTroopa_AnimatorComponent()
		: AnimatorComponent("greenkoopatroopa")
	{

	}

	~GreenKoopaTroopa_AnimatorComponent() {

	}

	void update() override {
		int timeslice = 0;

		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height - 20;

		sprite->srcRect.h = sprite->transform->height + 20;
		
		if (shelltoturtle)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			sprite->destRect.y = static_cast<int>(sprite->transform->position.y - 20) - Game::camera.y; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 2));
			if (timeslice == 1)// on finish
			{
				sprite->initTime = 0;
				shelltoturtle = false;
				Play("GreenKoopaTroopaWalk");
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->transform->velocity.x = -1;
			}
		}
		else
		{
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y - 20;
		}
		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale + 20;

	}
};