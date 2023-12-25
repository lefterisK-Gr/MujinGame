#pragma once

#include "../Components.h"


extern Manager manager;

class Sword : public Component
{
	int _attackDamage = 10;

public:

	TransformComponent* transform;
	ColliderComponent* collider;
	SpriteComponent* sprite;
	Slice* sliceComp;

	Sword()
	{

	}

	~Sword() {

	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		collider = &entity->getComponent<ColliderComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update(float deltaTime) override {

	}

	bool attack() {
		auto& slice(manager.addEntity());
		SDL_Rect relativeCollider = collider->collider;
		relativeCollider.x = sprite->spriteFlip == SDL_FLIP_NONE ? relativeCollider.x + relativeCollider.w : relativeCollider.x - relativeCollider.w;
		slice.addComponent<Slice>(relativeCollider, _attackDamage);
		sliceComp = &entity->getComponent<Slice>();
		slice.addGroup(Game::groupSlices);

		return true;
	}

};