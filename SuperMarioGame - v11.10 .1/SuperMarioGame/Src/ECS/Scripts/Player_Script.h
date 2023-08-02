#pragma once

#include "../Components.h"


class Player_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool vertTransitionPlayerAnimation = false;
	bool horTransitionPlayerAnimation = false;

	bool finishedVertAnimation = false;
	bool finishedHorAnimation = false;

	bool onPipe = false;
	bool leftofPipe = false;

	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	SpriteComponent* sprite;
	KeyboardControllerComponent* keyboard;

	Player_Script()
	{

	}

	~Player_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		moving_animator = &entity->getComponent<MovingAnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		keyboard = &entity->getComponent<KeyboardControllerComponent>();
	}

	void update() override {
		int timeslice = 0;
		if (!vertTransitionPlayerAnimation && !horTransitionPlayerAnimation) 
		{
			if (this->leftofPipe)
			{
				sprite->transform->velocity.x = 0;
				this->horTransitionPlayerAnimation = true;
				keyboard->action = KeyboardControllerComponent::playerAction::PLAYERACTION_JUMP;
			}
			if (keyboard->keystate[keyboard->downKey])
			{
				if (this->onPipe)
				{
					moving_animator->Play("PlayerVertTransition");
					this->vertTransitionPlayerAnimation = true;
					keyboard->action = KeyboardControllerComponent::playerAction::PLAYERACTION_JUMP;
				}
			}
		}
		if (vertTransitionPlayerAnimation || horTransitionPlayerAnimation) // transition on pipe
		{
			std::cout << "transision" << std::endl;
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}

			if (Game::justResumed)// if we just resumed
			{
				animator->resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			
			timeslice = sprite->animation.cur_frame_index; // for on pause
			if (timeslice == 16)// on finish
			{
				vertTransitionPlayerAnimation ? finishedVertAnimation = true : finishedHorAnimation = true;
				vertTransitionPlayerAnimation ? vertTransitionPlayerAnimation = false : horTransitionPlayerAnimation = false;
				sprite->initTime = 0;
			}
		}
	}

};