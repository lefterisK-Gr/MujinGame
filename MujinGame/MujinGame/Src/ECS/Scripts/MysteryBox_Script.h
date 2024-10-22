#pragma once

#include "ECS/Animators/AnimatorComponent.h"
#include "AudioEngine/AudioEngine.h"


class MysteryBox_Script: public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
public: // it is like it has init that creates Animator Component since it inherits it
	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	SpriteComponent* sprite = nullptr;

	MysteryBox_Script()
	{

	}

	~MysteryBox_Script() {

	}

	void init() override {
		animator = &entity->GetComponent<AnimatorComponent>();
		moving_animator = &entity->GetComponent<MovingAnimatorComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();

		animator->Play("QuestionMark");
	}

	void update(float deltaTime) override {
	}

};