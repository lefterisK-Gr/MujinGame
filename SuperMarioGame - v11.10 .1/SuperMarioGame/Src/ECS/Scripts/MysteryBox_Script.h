#pragma once

#include "../AnimatorComponent.h"


class MysteryBox_Script: public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool didCoinAnimation = false;

	AnimatorComponent* animator;
	SpriteComponent* sprite;

	MysteryBox_Script()
	{

	}

	MysteryBox_Script(std::string texID)
	{

	}

	~MysteryBox_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {

		int timeslice = 0;

		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height;

		if (didCoinAnimation) //add finished coin animation so its not checked everytime
		{
			if (!sprite->initTime) //this code is same for platform
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				animator->resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - animator->resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 3));
			sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) - (20 * timeslice) - Game::camera.y);
			if (timeslice == 3)
			{
				sprite->destRect.y += 40;
			}
			else if (timeslice == 4)
			{
				sprite->destRect.y += 80;
			}
			else if (timeslice == 5) // on finish
			{
				sprite->DestroyTex();
			}
		}
		else
		{
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;

		}
		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale;
	}

	void DestroyTex()
	{
		sprite->DestroyTex();
		didCoinAnimation = true;
	}

	bool getCoinAnimation()
	{
		return didCoinAnimation;
	}

};