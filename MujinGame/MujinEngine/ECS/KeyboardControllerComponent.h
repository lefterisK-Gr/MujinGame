#pragma once


#include "./Components.h"
#include <Windows.h>
#include <MMSystem.h>
#include "../InputManager/InputManager.h"

class KeyboardControllerComponent : public Component //! moving animation
{
public: //TODO: maybe have variables as private
	InputManager& _inputManager;

	TransformComponent* transform;
	AnimatorComponent* animator;
	RigidBodyComponent* rigidbody;
	SpriteComponent* sprite;

	char* idleAnimation, *jumpAnimation, *walkAnimation, *attackAnimation;
	SDL_KeyCode jumpKey, walkLeftKey, walkRightKey, attackKey,ability1Key, pickUpKey, inventoryKey, runKey, downKey;

	KeyboardControllerComponent(
		InputManager& inputManager,
		char* idleAnimation,
		char* jumpAnimation,
		char* walkAnimation,
		char* attackAnimation,
		char* ability1Animation,
		SDL_KeyCode jumpKey,
		SDL_KeyCode walkLeftKey,
		SDL_KeyCode walkRightKey,
		SDL_KeyCode attackKey,
		SDL_KeyCode ability1Key,
		SDL_KeyCode pickUpKey,
		SDL_KeyCode inventoryKey,
		SDL_KeyCode downKey,
		SDL_KeyCode runKey
		)

		: _inputManager(inputManager),
		idleAnimation(idleAnimation),
		jumpAnimation(jumpAnimation),
		walkAnimation(walkAnimation),
		attackAnimation(attackAnimation),
		jumpKey(jumpKey),
		walkLeftKey(walkLeftKey),
		walkRightKey(walkRightKey),
		attackKey(attackKey),
		ability1Key(ability1Key),
		pickUpKey(pickUpKey),
		inventoryKey(inventoryKey),
		downKey(downKey),
		runKey(runKey)
	{

	}

	~KeyboardControllerComponent()
	{

	}

	void init() override
	{
		transform = &entity->GetComponent<TransformComponent>();
		animator = &entity->GetComponent<AnimatorComponent>();
		rigidbody = &entity->GetComponent<RigidBodyComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
	}

	void update(float deltaTime) override
	{
		if (_inputManager.isKeyDown(jumpKey)) {
			if (rigidbody->onGround)
			{
				//PlaySound(TEXT("jump.wav"), NULL, SND_ASYNC);
				rigidbody->justjumped = true;
				transform->velocity.y = -3;
				//animation->Play(jumpAnimation.c_str()); // todo maybe not needing that because of !rigidbody->onGround
			}
		}

		if (_inputManager.isKeyDown(walkLeftKey)) {
			transform->velocity.x = -3.5;

			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		if (_inputManager.isKeyDown(walkRightKey)) {
			if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
			{
				animator->sprite->spriteFlip = SDL_FLIP_NONE;
			}

			transform->velocity.x = 3.5;
		}
		if (!_inputManager.isKeyDown(walkRightKey) && !_inputManager.isKeyDown(walkLeftKey)) {
			transform->velocity.x = 0;
		}
		if (!_inputManager.isKeyDown(runKey)) {
			if (_inputManager.isKeyDown(walkLeftKey)) {
				transform->velocity.x = -2.5;  // Set velocity to 1 (walking speed) in left direction
			}
			else if (_inputManager.isKeyDown(walkRightKey)) {
				transform->velocity.x = 2.5;   // Set velocity to 1 (walking speed) in right direction
			}
		}
	}
};