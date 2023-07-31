#pragma once


#include "./../Game.h"
#include "./ECS.h"
#include "./Components.h"
#include <Windows.h>
#include <MMSystem.h>
//class RigidBodyComponent;

class KeyboardControllerComponent : public Component //! moving animation
{
public: //TODO: maybe have variables as private
	TransformComponent* transform;
	AnimatorComponent* animator;
	Player_Script* script;
	RigidBodyComponent* rigidbody;
	const Uint8* keystate;

	char* idleAnimation, *jumpAnimation, *walkAnimation;
	SDL_Scancode jumpKey, walkLeftKey, walkRightKey, runKey, downKey;

	typedef enum {
		PLAYERACTION_IDLE = 0,
		PLAYERACTION_WALK = 1,
		PLAYERACTION_RUN = 2,
		PLAYERACTION_JUMP = 3
	} playerAction;

	playerAction action = playerAction::PLAYERACTION_IDLE;

	KeyboardControllerComponent()
	{

	}

	KeyboardControllerComponent(
		char* idleAnimation,
		char* jumpAnimation,
		char* walkAnimation,
		SDL_Scancode jumpKey,
		SDL_Scancode walkLeftKey,
		SDL_Scancode walkRightKey,
		SDL_Scancode downKey,
		SDL_Scancode runKey
		)

		: idleAnimation(idleAnimation),
		jumpAnimation(jumpAnimation),
		walkAnimation(walkAnimation),
		jumpKey(jumpKey),
		walkLeftKey(walkLeftKey),
		walkRightKey(walkRightKey),
		downKey(downKey),
		runKey(runKey)
	{

	}

	~KeyboardControllerComponent()
	{

	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		animator = &entity->getComponent<AnimatorComponent>();
		script = &entity->getComponent<Player_Script>();
		rigidbody = &entity->getComponent<RigidBodyComponent>();
	}

	void update() override
	{
		keystate = SDL_GetKeyboardState(NULL);
		if (!script->vertTransitionPlayerAnimation && !script->horTransitionPlayerAnimation)
		{
			if (Game::event.type == SDL_KEYDOWN)
			{
				if (keystate[jumpKey])
				{
					if (rigidbody->onGround)
					{
						//PlaySound(TEXT("jump.wav"), NULL, SND_ASYNC);
						rigidbody->justjumped = true;
						transform->velocity.y = -25;
						//animation->Play(jumpAnimation.c_str()); // todo maybe not needing that because of !rigidbody->onGround
					}
				}
				if (keystate[walkLeftKey])
				{
					transform->velocity.x = -2;

					script->sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				}
				if (keystate[walkRightKey])
				{
					if (script->sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
					{
						animator->sprite->spriteFlip = SDL_FLIP_NONE;
					}

					transform->velocity.x = 2;

					if (rigidbody->leftofPipe)
					{
						transform->velocity.x = 0;
						script->horTransitionPlayerAnimation = true;
						action = playerAction::PLAYERACTION_JUMP;
					}
				}
				if (keystate[runKey])
				{
					transform->velocity.x *= 2;
				}
				if (keystate[SDL_SCANCODE_S])
				{
					if (rigidbody->onPipe)
					{
						script->vertTransitionPlayerAnimation = true;
						action = playerAction::PLAYERACTION_JUMP;
					}
				}
			}
			if (Game::event.type == SDL_KEYUP)
			{
				if (!keystate[walkRightKey] && !keystate[walkLeftKey])
				{
					transform->velocity.x = 0;
					//animation->Play("P1Idle");
				}
				if (!keystate[runKey])
				{
					transform->velocity.x /= 2;
				}
			}
			
			if (!rigidbody->onGround)
			{
				if (action == playerAction::PLAYERACTION_JUMP)
					return;
				action = playerAction::PLAYERACTION_JUMP;
			}
			else if (rigidbody->onGround && transform->velocity.x == 0)
			{
				if (action == playerAction::PLAYERACTION_IDLE)
					return;
				action = playerAction::PLAYERACTION_IDLE;
			}
			else if (rigidbody->onGround && transform->velocity.x != 0)
			{
				if (action == playerAction::PLAYERACTION_WALK)
				{
					return;
				}
				action = playerAction::PLAYERACTION_WALK;
			}
			

			switch (action)
			{
			case playerAction::PLAYERACTION_IDLE:
				animator->Play(idleAnimation);
				break;
			case playerAction::PLAYERACTION_WALK:
				animator->Play(walkAnimation);
				break;
			case playerAction::PLAYERACTION_RUN:
				animator->Play(walkAnimation);
				break;
			case playerAction::PLAYERACTION_JUMP:
				animator->Play(jumpAnimation);
				break;
			default:
				break;
			}
		}
	}
};