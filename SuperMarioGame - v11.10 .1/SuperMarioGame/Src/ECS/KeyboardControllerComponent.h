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
	Player_AnimatorComponent* animation;
	RigidBodyComponent* rigidbody;
	const Uint8* keystate;

	char* idleAnimation, *jumpAnimation, *walkAnimation;
	SDL_Scancode jumpKey, walkLeftKey, walkRightKey, runKey, downKey;

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
		animation = &entity->getComponent<Player_AnimatorComponent>();
		rigidbody = &entity->getComponent<RigidBodyComponent>();
	}

	void update() override
	{
		keystate = SDL_GetKeyboardState(NULL);
		if (!animation->vertTransitionPlayerAnimation && !animation->horTransitionPlayerAnimation)
		{
			if (!rigidbody->onGround)
			{
				animation->Play(jumpAnimation);
			}
			else if (transform->velocity.x == 0)
			{
				animation->Play(idleAnimation);
			}
			else if (transform->velocity.x != 0)
			{
				animation->Play(walkAnimation);
			}
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
					if (rigidbody->onGround)
					{
						animation->Play(walkAnimation);
					}
					animation->sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				}
				if (keystate[walkRightKey])
				{
					if (animation->sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
					{
						animation->sprite->spriteFlip = SDL_FLIP_NONE;
					}

					transform->velocity.x = 2;
					if (rigidbody->onGround)
					{
						animation->Play(walkAnimation);
					}
					if (rigidbody->leftofPipe)
					{
						transform->velocity.x = 0;
						animation->horTransitionPlayerAnimation = true;
						animation->Play(jumpAnimation);
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
						animation->vertTransitionPlayerAnimation = true;
						animation->Play(jumpAnimation);
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
		}
	}
};