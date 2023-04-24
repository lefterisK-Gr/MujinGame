#pragma once

#include "../../Game.h"
#include "../ECS.h"
#include "../Components.h"
#include <Windows.h>
#include <MMSystem.h>
//class RigidBodyComponent;

class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	Player_AnimatorComponent* animation;
	RigidBodyComponent *rigidbody;
	const Uint8* keystate;

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
				animation->Play("P1Jump");
			}
			else if (transform->velocity.x == 0)
			{
				animation->Play("P1Idle");
			}
			else if (transform->velocity.x > 0)
			{
				animation->Play("P1Walk");
			}
			else if (transform->velocity.x < 0)
			{
				animation->Play("P1Walk");
			}
			if (Game::event.type == SDL_KEYDOWN)
			{
				if (keystate[SDL_SCANCODE_W])
				{
					if (rigidbody->onGround)
					{
						//PlaySound(TEXT("jump.wav"), NULL, SND_ASYNC);
						rigidbody->justjumped = true;
						transform->velocity.y = -25;
						animation->Play("P1Jump");
					}
				}
				if (keystate[SDL_SCANCODE_A])
				{
					if (keystate[SDL_SCANCODE_LSHIFT])
					{
						transform->velocity.x = -5;
					}
					else
					{
						transform->velocity.x = -2;
					}
					if (rigidbody->onGround)
					{
						animation->Play("P1Walk");
					}
					animation->sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				}
				if (keystate[SDL_SCANCODE_D])
				{
					if (animation->sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
					{
						animation->sprite->spriteFlip = SDL_FLIP_NONE;
					}
					if (keystate[SDL_SCANCODE_LSHIFT])
					{
						transform->velocity.x = 5;
					}
					else
					{
						transform->velocity.x = 2;
					}
					if (rigidbody->onGround)
					{
						animation->Play("P1Walk");
					}
					if (rigidbody->leftofPipe)
					{
						transform->velocity.x = 0;
						animation->horTransitionPlayerAnimation = true;
						animation->Play("P1Jump");
					}
				}
				if (keystate[SDL_SCANCODE_S])
				{
					if (rigidbody->onPipe)
					{
						animation->vertTransitionPlayerAnimation = true;
						animation->Play("P1Jump");
					}
				}
			}
			if (Game::event.type == SDL_KEYUP)
			{
				if (!keystate[SDL_SCANCODE_D] && !keystate[SDL_SCANCODE_A])
				{
					transform->velocity.x = 0;
					//animation->Play("P1Idle");
				}
				if (!keystate[SDL_SCANCODE_LSHIFT] && (transform->velocity.x < 0))
				{
					transform->velocity.x = -2;
				}
				if (!keystate[SDL_SCANCODE_LSHIFT] && (transform->velocity.x > 0))
				{
					transform->velocity.x = 2;
				}
			}
		}
	}
};
