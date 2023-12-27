#pragma once

#include "../Components.h"

class GreenKoopaTroopa_Script : public Component
{
public:
	bool shelltoturtle = false;
	AnimatorComponent* animator;
	SpriteComponent* sprite = nullptr;
	TransformComponent* transform = nullptr;
	LivingCharacter* living;

	GreenKoopaTroopa_Script()
	{

	}

	~GreenKoopaTroopa_Script() {

	}

	void init() override {
		animator = &entity->getComponent<AnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		transform = &entity->getComponent<TransformComponent>();
		if (!entity->hasComponent<LivingCharacter>()) //todo: problem: having transform on top left grid, not every collider its own
		{
			entity->addComponent<LivingCharacter>();
		}
		living = &entity->getComponent<LivingCharacter>();
	}

	void update(float deltaTime) override {

		if (transform->velocity.x < 0) {
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else {
			sprite->spriteFlip = SDL_FLIP_NONE;
		}

		if (shelltoturtle)
		{
			if (sprite->transform->velocity.x != 0) {
				sprite->animation.resetFrameIndex();
			}
			if (sprite->animation.hasFinished()) {
				shelltoturtle = false;
				animator->Play("GreenKoopaTroopaWalk");
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->transform->velocity.x = -1;
			}
		}

	}
};