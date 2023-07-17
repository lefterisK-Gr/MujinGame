#pragma once

#include "../Components.h"


class Player_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool vertTransitionPlayerAnimation = false;
	bool horTransitionPlayerAnimation = false;

	bool finishedVertAnimation = false;
	bool finishedHorAnimation = false;

	AnimatorComponent* animator;
	SpriteComponent* sprite;

	Player_Script()
	{

	}

	~Player_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		int timeslice = 0;

		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height;

		if (vertTransitionPlayerAnimation || horTransitionPlayerAnimation) // transition on pipe
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			vertTransitionPlayerAnimation  ? sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x : sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y; ; //make player move with the camera, being stable in centre, except on edges

			if (Game::justResumed)// if we just resumed
			{
				animator->resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			
			timeslice = static_cast<int>(((SDL_GetTicks() - animator->resumeTime - sprite->initTime) / sprite->speed)); // for on pause
			vertTransitionPlayerAnimation ? sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) + (2 * timeslice) - Game::camera.y) : sprite->destRect.x = (static_cast<int>(sprite->transform->position.x) + (2 * timeslice) - Game::camera.x);
			if (timeslice == 16)// on finish
			{
				vertTransitionPlayerAnimation ? finishedVertAnimation = true : finishedHorAnimation = true;
				vertTransitionPlayerAnimation ? vertTransitionPlayerAnimation = false : horTransitionPlayerAnimation = false;
				sprite->initTime = 0;
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

};