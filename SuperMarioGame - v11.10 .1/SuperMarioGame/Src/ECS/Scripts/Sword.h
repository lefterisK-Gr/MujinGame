#pragma once

#include "../Components.h"


extern Manager manager;
class LivingCharacter;


#include "LivingCharacter.h"
class Sword : public Component
{
	int _attackDamage = 10;
public:

	TransformComponent* transform = nullptr;
	ColliderComponent* collider = nullptr;
	SpriteComponent* sprite = nullptr;
	//LivingCharacter* livingCharacter = nullptr;
	Slice* sliceComp = nullptr;

	SDL_Rect hitBoxCollider = { 0, 0, 48, 48 };
	bool enemySlice = false;

	Sword()
	{

	}

	Sword(bool enemyS)
	{
		enemySlice = enemyS;
	}

	~Sword() {

	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		collider = &entity->getComponent<ColliderComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		//livingCharacter = &entity->getComponent<LivingCharacter>();
	}

	void update(float deltaTime) override {
		SDL_Rect relativeCollider = collider->collider;
		hitBoxCollider.x = sprite->spriteFlip == SDL_FLIP_NONE ?
			relativeCollider.x + relativeCollider.w :
			relativeCollider.x - hitBoxCollider.w;
		hitBoxCollider.y = relativeCollider.y;
	}

	bool attack() {
		auto& slice(manager.addEntity());
		slice.addComponent<Slice>(hitBoxCollider, _attackDamage);
		sliceComp = &entity->getComponent<Slice>();
		slice.addGroup(enemySlice ? Game::groupEnemySlices : Game::groupSlices);

		return true;
	}

	bool ability1() {
		if (entity->hasComponent<LivingCharacter>()) {
			if (!entity->getComponent<LivingCharacter>().applyMana(10)) {
				Vector2D centerTransform = transform->getCenterTransform();
				Vector2D projectileDirection = sprite->spriteFlip == SDL_FLIP_NONE ? Vector2D(1, 0) : Vector2D(-1, 0);
				Vector2D projectileStartPosition = centerTransform;
				projectileStartPosition.Add(projectileDirection);
				Game::assets->CreateProjectile(centerTransform, projectileStartPosition, 300, 3, "warriorProjectile");
			}
		}

		return true;
	}

};