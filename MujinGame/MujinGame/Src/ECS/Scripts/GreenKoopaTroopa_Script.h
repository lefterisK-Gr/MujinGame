#pragma once

#include "../Components.h"
#include "AudioEngine/AudioEngine.h"

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
		else if (transform->velocity.x > 0) {
			sprite->spriteFlip = SDL_FLIP_NONE;
		}

		if (shelltoturtle)
		{
			if (sprite->animation.hasFinished()) {
				shelltoturtle = false;
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->transform->velocity.x = -1;
			}
		}

		if (attackAnimation) {
			if (sprite->animation.hasFinished()) {
				this->attackAnimation = false;
				this->action = GreenKoopaTroopa_Script::greenKoopaTroopaAction::KOOPAACTION_IDLE;
				transform->velocity.x = 1;
			}
		}

		if (action == greenKoopaTroopaAction::KOOPAACTION_ATTACK) {
			if (sprite->animation.cur_frame_index == 6 && sprite->animation.frame_times_played == 1)
			{
				auto& players(manager.getGroup(Game::groupPlayers));

				for (auto& p : players)
				{
					auto enemyTransform = *transform;
					auto playerTransform = p->getComponent<TransformComponent>();

					if ((enemyTransform.position.x < playerTransform.position.x + 200 && enemyTransform.position.x > playerTransform.position.x) ||
						(enemyTransform.position.x > playerTransform.position.x - 200 && enemyTransform.position.x < playerTransform.position.x))
					{
						_projectileEffect.play();
						Game::assets->CreateProjectile(enemyTransform.getCenterTransform(), playerTransform.getCenterTransform(), 300, 3, "projectile");
					}
				}
			}
			return;
		}
		else if (transform->velocity.x != 0)
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