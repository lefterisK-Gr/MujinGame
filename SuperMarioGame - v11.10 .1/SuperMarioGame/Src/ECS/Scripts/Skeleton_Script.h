#pragma once

#include "../Components.h"

class Skeleton_Script : public Component
{
public:
	SpriteComponent* sprite;
	TransformComponent* transform;
	LivingCharacter* living;

	Skeleton_Script()
	{

	}

	~Skeleton_Script() {

	}

	void init() override {
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
	}

};