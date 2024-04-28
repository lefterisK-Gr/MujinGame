#pragma once

#include "ECS/AnimatorComponent.h"


class PlatformBlock_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool didBlockAnimation = false;

	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	SpriteComponent* sprite = nullptr;

	PlatformBlock_Script()
	{

	}

	PlatformBlock_Script(std::string texID)
	{

	}

	~PlatformBlock_Script() {

	}

	void init() override {
		animator = &entity->GetComponent<AnimatorComponent>();
		moving_animator = &entity->GetComponent<MovingAnimatorComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
	}

	void update(float deltaTime) override {

		if (didBlockAnimation)
		{
			moving_animator->Play("BlockBounce");

			didBlockAnimation = false;
		}
	}

};