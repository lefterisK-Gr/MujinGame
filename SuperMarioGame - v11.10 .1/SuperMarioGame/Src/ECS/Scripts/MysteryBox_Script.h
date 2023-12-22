#pragma once

#include "../AnimatorComponent.h"


class MysteryBox_Script: public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	bool doCoinAnimation = false;
	bool lockCoinAnimation = false;

	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	SpriteComponent* sprite;

	MysteryBox_Script()
	{

	}

	MysteryBox_Script(std::string texID)
	{

	}

	~MysteryBox_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		moving_animator = &entity->getComponent<MovingAnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		if (doCoinAnimation && !lockCoinAnimation) //add finished coin animation so its not checked everytime
		{
			moving_animator->Play("CoinBounce");

			doCoinAnimation = false;
			lockCoinAnimation = true;
		}

		if (sprite->moving_animation.hasFinished()) {
			sprite->DestroyGlTex();
		}
	}

	bool getCoinAnimation()
	{
		return doCoinAnimation;
	}

};