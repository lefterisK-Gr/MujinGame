#pragma once

#include "../AnimatorComponent.h"


class PlatformBlock_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool didBlockAnimation = false;

	AnimatorComponent* animator;
	SpriteComponent* sprite;

	PlatformBlock_Script()
	{

	}

	PlatformBlock_Script(std::string texID)
	{

	}

	~PlatformBlock_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {

		int timeslice = 0;

		if (didBlockAnimation)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				animator->resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - animator->resumeTime - sprite->initTime) / (int)sprite->animation.speed) % (sprite->animation.total_frames + 4));
			sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) - (8 * (timeslice + 1)) - Game::camera.y);

			if (timeslice == 2)
			{
				sprite->destRect.y += 16;
			}
			else if (timeslice == 3)// on finish
			{
				sprite->destRect.y += 32;
				sprite->initTime = 0;
				didBlockAnimation = false;
			}
		}
	}

};