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
	TransformComponent* transform;
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
		transform = &entity->getComponent<TransformComponent>();
		keyboard = &entity->getComponent<KeyboardControllerComponent>();
	}

	void update() override {
		if (!vertTransitionPlayerAnimation && !horTransitionPlayerAnimation) 
		{
			if (keyboard->keystate[keyboard->walkRightKey])
			{
				if (this->leftofPipe)
				{
					sprite->transform->velocity.x = 0;
					moving_animator->Play("PlayerHorTransition");
					this->horTransitionPlayerAnimation = true;
					keyboard->action = KeyboardControllerComponent::playerAction::PLAYERACTION_JUMP;
				}
			}
			if (keyboard->keystate[keyboard->downKey])
			{
				if (this->onPipe)
				{
					sprite->transform->velocity.x = 0;
					moving_animator->Play("PlayerVertTransition");
					this->vertTransitionPlayerAnimation = true;
					keyboard->action = KeyboardControllerComponent::playerAction::PLAYERACTION_JUMP;
				}
			}
		}
		if (vertTransitionPlayerAnimation || horTransitionPlayerAnimation) // transition on pipe
		{	
			if (sprite->moving_animation.hasFinished()) {
				if (vertTransitionPlayerAnimation) {
					finishedVertAnimation = true;
					vertTransitionPlayerAnimation = false;
				}
				else {
					finishedHorAnimation = true;
					horTransitionPlayerAnimation = false;
				}
			}
		}

		onPipe = false;
		leftofPipe = false;
	}

};