#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

//class RigidBodyComponent;

class KeyboardController2 : public Component
{
public:
	TransformComponent* transform;
	AnimatorComponent* animation;
	RigidBodyComponent* rigidbody;
	const Uint8* keystate;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		animation = &entity->getComponent<AnimatorComponent>();
		rigidbody = &entity->getComponent<RigidBodyComponent>();
	}

	void update() override
	{
		keystate = SDL_GetKeyboardState(NULL);
		if (!animation->vertTransitionPlayerAnimation && !animation->horTransitionPlayerAnimation)
		{
			if (!rigidbody->onGround)
			{
				animation->Play("P2Jump");
			}
			else if (transform->velocity.x == 0)
			{
				animation->Play("P2Idle");
			}
			else if (transform->velocity.x > 0)
			{
				animation->Play("P2Walk");
			}
			else if (transform->velocity.x < 0)
			{
				animation->Play("P2Walk");
			}
			if (Game::event.type == SDL_KEYDOWN)
			{
				if (keystate[SDL_SCANCODE_UP])
				{
					if (rigidbody->onGround)
					{
						//PlaySound(TEXT("jump.wav"), NULL, SND_ASYNC);
						rigidbody->justjumped = true;
						transform->velocity.y = -25;
						animation->Play("P2Jump");
					}
				}
				if (keystate[SDL_SCANCODE_LEFT])
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
						animation->Play("P2Walk");
					}
					animation->sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				}
				if (keystate[SDL_SCANCODE_RIGHT])
				{
					if (animation->sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
					{
						animation->sprite->spriteFlip = SDL_FLIP_NONE;
					}
					if (keystate[SDL_SCANCODE_L])
					{
						transform->velocity.x = 5;
					}
					else
					{
						transform->velocity.x = 2;
					}
					if (rigidbody->onGround)
					{
						animation->Play("P2Walk");
					}
					if (rigidbody->leftofPipe)
					{
						transform->velocity.x = 0;
						animation->horTransitionPlayerAnimation = true;
						animation->Play("P2Jump");
					}
				}
			}
			if (keystate[SDL_SCANCODE_DOWN])
			{
				if (rigidbody->onPipe)
				{
					animation->vertTransitionPlayerAnimation = true;
					animation->Play("P2Jump");
				}
			}
			if (Game::event.type == SDL_KEYUP)
			{
				if (!keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT])
				{
					transform->velocity.x = 0;
					//animation->Play("P2Idle");
				}
				if (!keystate[SDL_SCANCODE_L] && (transform->velocity.x < 0))
				{
					transform->velocity.x = -2;
				}
				if (!keystate[SDL_SCANCODE_L] && (transform->velocity.x > 0))
				{
					transform->velocity.x = 2;
				}
			}
		}
	}
};