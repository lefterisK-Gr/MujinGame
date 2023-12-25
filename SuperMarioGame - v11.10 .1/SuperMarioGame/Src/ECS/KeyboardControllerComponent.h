#pragma once


#include "./../Game.h"
#include "./ECS.h"
#include "./Components.h"
#include <Windows.h>
#include <MMSystem.h>
#include "../Game.h"
//class RigidBodyComponent;

class KeyboardControllerComponent : public Component //! moving animation
{
public: //TODO: maybe have variables as private
	TransformComponent* transform;
	AnimatorComponent* animator;
	RigidBodyComponent* rigidbody;
	SpriteComponent* sprite;

	char* idleAnimation, *jumpAnimation, *walkAnimation, *attackAnimation;
	SDL_KeyCode jumpKey, walkLeftKey, walkRightKey, attackKey, runKey, downKey;

	KeyboardControllerComponent()
	{

	}

	KeyboardControllerComponent(
		char* idleAnimation,
		char* jumpAnimation,
		char* walkAnimation,
		char* attackAnimation,
		SDL_KeyCode jumpKey,
		SDL_KeyCode walkLeftKey,
		SDL_KeyCode walkRightKey,
		SDL_KeyCode attackKey,
		SDL_KeyCode downKey,
		SDL_KeyCode runKey
		)

		: idleAnimation(idleAnimation),
		jumpAnimation(jumpAnimation),
		walkAnimation(walkAnimation),
		attackAnimation(attackAnimation),
		jumpKey(jumpKey),
		walkLeftKey(walkLeftKey),
		walkRightKey(walkRightKey),
		attackKey(attackKey),
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
		rigidbody = &entity->getComponent<RigidBodyComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update(float deltaTime) override
	{
		if (Game::_inputManager.isKeyDown(jumpKey)) {
			if (rigidbody->onGround)
			{
				//PlaySound(TEXT("jump.wav"), NULL, SND_ASYNC);
				rigidbody->justjumped = true;
				transform->velocity.y = -7;
				//animation->Play(jumpAnimation.c_str()); // todo maybe not needing that because of !rigidbody->onGround
			}
		}

		if (Game::_inputManager.isKeyDown(walkLeftKey)) {
			transform->velocity.x = -2;

			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		if (Game::_inputManager.isKeyDown(walkRightKey)) {
			if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
			{
				animator->sprite->spriteFlip = SDL_FLIP_NONE;
			}

			transform->velocity.x = 2;
		}
		if (Game::_inputManager.isKeyDown(runKey)) {
			transform->velocity.x *= 2;
		}
		if (!Game::_inputManager.isKeyDown(walkRightKey) && !Game::_inputManager.isKeyDown(walkLeftKey)) {
			transform->velocity.x = 0;
		}
		if (!Game::_inputManager.isKeyDown(runKey)) {
			if (Game::_inputManager.isKeyDown(walkLeftKey)) {
				transform->velocity.x = -1;  // Set velocity to 1 (walking speed) in left direction
			}
			else if (Game::_inputManager.isKeyDown(walkRightKey)) {
				transform->velocity.x = 1;   // Set velocity to 1 (walking speed) in right direction
			}
		}
	}
};