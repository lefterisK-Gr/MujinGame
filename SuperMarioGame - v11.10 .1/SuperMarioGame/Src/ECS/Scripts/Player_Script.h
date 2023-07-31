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

		if (vertTransitionPlayerAnimation || horTransitionPlayerAnimation) // transition on pipe
		{
			std::cout << "transision" << std::endl;
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			vertTransitionPlayerAnimation  ? sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x : sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y; ; //make player move with the camera, being stable in centre, except on edges

			if (Game::justResumed)// if we just resumed
			{
				animator->resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			
			timeslice = sprite->animation.cur_frame_index; // for on pause
			vertTransitionPlayerAnimation ? sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) + (2 * timeslice) - Game::camera.y) : sprite->destRect.x = (static_cast<int>(sprite->transform->position.x) + (2 * timeslice) - Game::camera.x);
			if (timeslice == 16)// on finish
			{
				vertTransitionPlayerAnimation ? finishedVertAnimation = true : finishedHorAnimation = true;
				vertTransitionPlayerAnimation ? vertTransitionPlayerAnimation = false : horTransitionPlayerAnimation = false;
				sprite->initTime = 0;
			}
		}
	}

};