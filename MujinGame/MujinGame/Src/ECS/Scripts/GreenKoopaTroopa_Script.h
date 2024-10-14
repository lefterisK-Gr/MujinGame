#pragma once

#include "ECS/Components.h"
#include "../../AssetManager/AssetManager.h"
#include "AudioEngine/AudioEngine.h"
#include "../../Collision/Collision.h"

class GreenKoopaTroopa_Script : public Component
{
private:
	SoundEffect _projectileEffect = Game::audioEngine.loadSoundEffect("Sounds/enemyThrowProjectile.wav");
public:
	bool attackAnimation = false;
	bool shelltoturtle = false;
	AnimatorComponent* animator;
	SpriteComponent* sprite = nullptr;
	TransformComponent* transform = nullptr;
	LivingCharacter* living;
	ColliderComponent* collider;

	typedef enum {
		KOOPAACTION_IDLE = 0,
		KOOPAACTION_WALK = 1,
		KOOPAACTION_ATTACK = 2
	} greenKoopaTroopaAction;

	greenKoopaTroopaAction action = greenKoopaTroopaAction::KOOPAACTION_WALK;

	GreenKoopaTroopa_Script()
	{

	}

	~GreenKoopaTroopa_Script() {

	}

	void init() override {
		animator = &entity->GetComponent<AnimatorComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
		transform = &entity->GetComponent<TransformComponent>();
		if (!entity->hasComponent<LivingCharacter>()) //todo: problem: having transform on top left grid, not every collider its own
		{
			entity->addComponent<LivingCharacter>();
		}
		living = &entity->GetComponent<LivingCharacter>();
		collider = &entity->GetComponent<ColliderComponent>();
	}

	void update(float deltaTime) override {
		auto& warriorprojectiles(manager.getGroup(Manager::groupWarriorProjectiles));
		auto& slices(manager.getGroup(Manager::groupSlices));
		auto& players(manager.getGroup(Manager::groupPlayers));

		for (auto& sl : slices)
		{
			if (Collision::checkCollision(sl->GetComponent<ColliderComponent>().collider, entity->GetComponent<ColliderComponent>().collider))
			{
				for (auto& pl : players) {
					living->applyDamage(sl->GetComponent<Slice>().sliceDamage);
				}
			}
		}

		for (auto& wpr : warriorprojectiles) {
			if (Collision::checkCollision(entity->GetComponent<ColliderComponent>().collider, wpr->GetComponent<ColliderComponent>().collider))
			{
				for (auto& pl : players) {
					entity->GetComponent<LivingCharacter>().applyDamage(1);
				}
			}
		}

		if (transform->getVelocity().x < 0) {
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else if (transform->getVelocity().x > 0) {
			sprite->spriteFlip = SDL_FLIP_NONE;
		}

		if (shelltoturtle)
		{
			if (sprite->animation.hasFinished()) {
				shelltoturtle = false;
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->transform->setVelocity_X(-1) ;
			}
		}

		if (attackAnimation) {
			if (sprite->animation.hasFinished()) {
				this->attackAnimation = false;
				this->action = GreenKoopaTroopa_Script::greenKoopaTroopaAction::KOOPAACTION_IDLE;
				transform->setVelocity_X(1);
			}
		}

		if (action == greenKoopaTroopaAction::KOOPAACTION_ATTACK) {
			if (sprite->animation.cur_frame_index == 6 && sprite->animation.frame_times_played == 1)
			{
				auto& players(manager.getGroup(Manager::groupPlayers));

				for (auto& p : players)
				{
					auto enemyTransform = *transform;
					auto playerTransform = p->GetComponent<TransformComponent>();

					if ((enemyTransform.getPosition().x < playerTransform.getPosition().x + 200 && enemyTransform.getPosition().x > playerTransform.getPosition().x) ||
						(enemyTransform.getPosition().x > playerTransform.getPosition().x - 200 && enemyTransform.getPosition().x < playerTransform.getPosition().x))
					{
						_projectileEffect.play();
						Game::assets->CreateProjectile(enemyTransform.getCenterTransform(), playerTransform.getCenterTransform(), 300, 3, "projectile");
					}
				}
			}
			return;
		}
		else if (transform->getVelocity().x != 0)
		{
			if (action == greenKoopaTroopaAction::KOOPAACTION_WALK)
				return;
			action = greenKoopaTroopaAction::KOOPAACTION_WALK;
		}

		switch (action)
		{
		case greenKoopaTroopaAction::KOOPAACTION_IDLE:
			animator->Play("GreenKoopaTroopaIdle");
			break;
		case greenKoopaTroopaAction::KOOPAACTION_WALK:
			animator->Play("GreenKoopaTroopaWalk");
			break;
		case greenKoopaTroopaAction::KOOPAACTION_ATTACK:
			animator->Play("GreenKoopaTroopaAttack");
			break;
		default:
			break;
		}
	}

	void activateShoot() {
		if (!attackAnimation) {
			animator->Play("GreenKoopaTroopaAttack");
			attackAnimation = true;
			this->action = GreenKoopaTroopa_Script::greenKoopaTroopaAction::KOOPAACTION_ATTACK;
		}
	}
};