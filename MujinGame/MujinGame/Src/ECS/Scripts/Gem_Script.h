#pragma once

#include "ECS/Animators/AnimatorComponent.h"
#include "AudioEngine/AudioEngine.h"


class Gem_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
private:
	SoundEffect _gemEffect;
public: // it is like it has init that creates Animator Component since it inherits it
	bool lockCoinAnimation = false;

	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	SpriteComponent* sprite = nullptr;

	Gem_Script()
	{

	}

	Gem_Script(SoundEffect gemEffect)
	{
		_gemEffect = gemEffect;
	}

	~Gem_Script() {

	}

	void init() override {
		animator = &entity->GetComponent<AnimatorComponent>();
		moving_animator = &entity->GetComponent<MovingAnimatorComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();

		animator->Play("CoinFlip");
	}

	void update(float deltaTime) override {

		if (sprite->moving_animation.hasFinished() && lockCoinAnimation) {
			entity->destroy();
		}
	}

	void doCoinAnimation() {
		_gemEffect.play();
		moving_animator->Play("CoinBounce");

		lockCoinAnimation = true;
	}
};