#pragma once

#include "../AnimatorComponent.h"


class PlatformBlock_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool didBlockAnimation = false;

	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	SpriteComponent* sprite;

	PlatformBlock_Script()
	{

	}

	PlatformBlock_Script(std::string texID)
	{

	}

	~PlatformBlock_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		moving_animator = &entity->getComponent<MovingAnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update(float deltaTime) override {

		if (didBlockAnimation)
		{
			moving_animator->Play("BlockBounce");

			didBlockAnimation = false;
		}
	}

};