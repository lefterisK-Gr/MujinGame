#pragma once

#include "ECS/Components.h"
#include "AudioEngine/AudioEngine.h"


class Skeleton_Script : public Component
{
private:
	bool isGiant = false;
	SoundEffect _slashEffect = Game::audioEngine.loadSoundEffect("Sounds/enemySlash.wav");
public:

	bool attackAnimation = false;

	SpriteComponent* sprite = nullptr;
	TransformComponent* transform = nullptr;
	AnimatorComponent* animator;
	LivingCharacter* living;
	Sword* sword;

	typedef enum {
		SKELETONACTION_IDLE = 0,
		SKELETONACTION_WALK = 1,
		SKELETONACTION_ATTACK = 2
	} skeletonAction;

	skeletonAction action = skeletonAction::SKELETONACTION_IDLE;

	Skeleton_Script()
	{

	}

	Skeleton_Script(bool isG)
	{
		isGiant = isG;
	}

	~Skeleton_Script() {

	}

	void init() override {
		sprite = &entity->getComponent<SpriteComponent>();
		transform = &entity->getComponent<TransformComponent>();
		sword = &entity->getComponent<Sword>();
		animator = &entity->getComponent<AnimatorComponent>();

		if (!entity->hasComponent<LivingCharacter>()) //todo: problem: having transform on top left grid, not every collider its own
		{
			entity->addComponent<LivingCharacter>();
		}
		living = &entity->getComponent<LivingCharacter>();
		if (isGiant)
		{
			transform->speed *= 2;
			animator->animManager.animations.find("SkeletonAttack")->second.speed *= 2;
			living->hp_bar->getComponent<HPBar>()._healthPoints = 200;
			living->hp_bar->getComponent<HPBar>()._maxHealthPoints = 200;
		}
	}

	void update(float deltaTime) override {
		if (transform->velocity.x < 0) {
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else if(transform->velocity.x > 0){
			sprite->spriteFlip = SDL_FLIP_NONE;
		}

		if(attackAnimation) {
			if (sprite->animation.hasFinished()) {
				this->attackAnimation = false;
				this->action = Skeleton_Script::skeletonAction::SKELETONACTION_IDLE;
				transform->velocity.x = 1;
			}
		}

		if (action == skeletonAction::SKELETONACTION_ATTACK) { //if playerAttackAnimation is on 3rd frame then deal damage i.e create entity from sword that deal damage.
			if (sprite->animation.cur_frame_index == 6 && sprite->animation.frame_times_played == 1)
			{
				_slashEffect.play();
				sword->attack();
			}
			return;
		}
		else if (transform->velocity.x != 0)
		{
			if (action == skeletonAction::SKELETONACTION_WALK)
				return;
			action = skeletonAction::SKELETONACTION_WALK;
		}

		switch (action)
		{
		case skeletonAction::SKELETONACTION_IDLE:
			animator->Play("SkeletonIdle");
			break;
		case skeletonAction::SKELETONACTION_WALK:
			animator->Play("SkeletonWalk");
			break;
		case skeletonAction::SKELETONACTION_ATTACK:
			animator->Play("SkeletonAttack");
			break;
		default:
			break;
		}
	}

	void activateAttack() {
		if (!attackAnimation) {
			animator->Play("SkeletonAttack");
			this->attackAnimation = true;
			this->action = Skeleton_Script::skeletonAction::SKELETONACTION_ATTACK;
		}
	}
};