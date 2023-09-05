#pragma once

#include "../Components.h"


class Player_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool attackAnimation = false;

	bool vertTransitionPlayerAnimation = false;
	bool horTransitionPlayerAnimation = false;

	bool finishedVertAnimation = false;
	bool finishedHorAnimation = false;

	bool onPipe = false;
	bool leftofPipe = false;

	typedef enum {
		PLAYERACTION_IDLE = 0,
		PLAYERACTION_WALK = 1,
		PLAYERACTION_RUN = 2,
		PLAYERACTION_JUMP = 3,
		PLAYERACTION_ATTACK = 4
	} playerAction;

	playerAction action = playerAction::PLAYERACTION_IDLE;

	RigidBodyComponent* rigidbody;
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
		rigidbody = &entity->getComponent<RigidBodyComponent>();
		animator = &entity->getComponent<AnimatorComponent>();
		moving_animator = &entity->getComponent<MovingAnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		transform = &entity->getComponent<TransformComponent>();
		keyboard = &entity->getComponent<KeyboardControllerComponent>();
	}

	void update() override {
		if (!attackAnimation) {
			if (keyboard->keystate[keyboard->attackKey]) {
				animator->Play("P1Attack", 1);
				this->attackAnimation = true;
				this->action = Player_Script::playerAction::PLAYERACTION_ATTACK;
			}
		}

		if (attackAnimation) {
			if (sprite->animation.hasFinished()) {
				this->attackAnimation = false;
				this->action = Player_Script::playerAction::PLAYERACTION_IDLE;
			}
		}

		if (!vertTransitionPlayerAnimation && !horTransitionPlayerAnimation) 
		{
			if (keyboard->keystate[keyboard->walkRightKey])
			{
				if (this->leftofPipe)
				{
					sprite->transform->velocity.x = 0;
					moving_animator->Play("PlayerHorTransition", 1);
					this->horTransitionPlayerAnimation = true;
					this->action = Player_Script::playerAction::PLAYERACTION_JUMP;
				}
			}
			if (keyboard->keystate[keyboard->downKey])
			{
				if (this->onPipe)
				{
					sprite->transform->velocity.x = 0;
					moving_animator->Play("PlayerVertTransition", 1);
					this->vertTransitionPlayerAnimation = true;
					this->action = Player_Script::playerAction::PLAYERACTION_JUMP;
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

		if (action == playerAction::PLAYERACTION_ATTACK)
			return;
		else if (!rigidbody->onGround)
		{
			if (action == playerAction::PLAYERACTION_JUMP)
				return;
			action = playerAction::PLAYERACTION_JUMP;
		}
		else if (rigidbody->onGround && transform->velocity.x == 0 && action != playerAction::PLAYERACTION_ATTACK)
		{
			if (action == playerAction::PLAYERACTION_IDLE)
				return;
			action = playerAction::PLAYERACTION_IDLE;
		}
		else if (rigidbody->onGround && transform->velocity.x != 0)
		{
			if (action == playerAction::PLAYERACTION_WALK)
				return;
			action = playerAction::PLAYERACTION_WALK;
		}

		switch (action)
		{
		case playerAction::PLAYERACTION_IDLE:
			animator->Play(keyboard->idleAnimation);
			break;
		case playerAction::PLAYERACTION_WALK:
			animator->Play(keyboard->walkAnimation);
			break;
		case playerAction::PLAYERACTION_RUN:
			animator->Play(keyboard->walkAnimation);
			break;
		case playerAction::PLAYERACTION_JUMP:
			animator->Play(keyboard->jumpAnimation);
			break;
		case playerAction::PLAYERACTION_ATTACK:
			animator->Play(keyboard->attackAnimation, 1);
			break;
		default:
			break;
		}

		onPipe = false;
		leftofPipe = false;
	}

};