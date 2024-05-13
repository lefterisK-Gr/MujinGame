#pragma once

#include "ECS/Components.h"
#include "../../AssetManager/AssetManager.h"


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
		transform = &entity->GetComponent<TransformComponent>();
		collider = &entity->GetComponent<ColliderComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
		//livingCharacter = &entity->GetComponent<LivingCharacter>();
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
		sliceComp = &entity->GetComponent<Slice>();
		slice.addGroup(enemySlice ? Manager::groupEnemySlices : Manager::groupSlices);

		return true;
	}

	bool ability1() {
		if (entity->hasComponent<LivingCharacter>()) {
			if (!entity->GetComponent<LivingCharacter>().applyMana(10)) {
				Vector2D centerTransform = transform->getCenterTransform();
				Vector2D projectileDirection = sprite->spriteFlip == SDL_FLIP_NONE ? Vector2D(1, 0) : Vector2D(-1, 0);
				Vector2D projectileStartPosition = centerTransform;
				projectileStartPosition.Add(projectileDirection);
				Game::assets->CreateProjectile(centerTransform, projectileStartPosition, 300, 3, "warriorProjectile");
			}
		}

		return true;
	}

	void levelUpAttackDamage(int amount) {
		_attackDamage += amount;
	}

};