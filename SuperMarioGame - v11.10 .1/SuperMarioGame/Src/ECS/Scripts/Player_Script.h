#pragma once

#include "../Components.h"
#include "../../AudioEngine/AudioEngine.h"

class Player_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
private:
	SoundEffect _slashEffect = Game::audioEngine.loadSoundEffect("Sounds/slash.wav");
	SoundEffect _slashSkillEffect = Game::audioEngine.loadSoundEffect("Sounds/skillSlash.wav");
	SoundEffect _takeHitEffect = Game::audioEngine.loadSoundEffect("Sounds/takeHit.wav");


public: // it is like it has init that creates Animator Component since it inherits it
	bool attackAnimation = false;

	bool vertTransitionPlayerAnimation = false;
	bool horTransitionPlayerAnimation = false;

	bool finishedVertAnimation = false;
	bool finishedHorAnimation = false;

	bool onPipe = false;
	bool leftofPipe = false;

	bool tookDamage = false;

	typedef enum {
		PLAYERACTION_IDLE = 0,
		PLAYERACTION_WALK = 1,
		PLAYERACTION_RUN = 2,
		PLAYERACTION_JUMP = 3,
		PLAYERACTION_ATTACK = 4,
		PLAYERACTION_ABILITY_1 = 5
	} playerAction;

	playerAction action = playerAction::PLAYERACTION_IDLE;

	RigidBodyComponent* rigidbody;
	AnimatorComponent* animator;
	MovingAnimatorComponent* moving_animator;
	FlashAnimatorComponent* flash_animator;
	SpriteComponent* sprite;
	TransformComponent* transform;
	KeyboardControllerComponent* keyboard;
	Sword* sword;

	Entity* light;

	LivingCharacter* living;

	Player_Script()
	{

	}

	~Player_Script() {

	}

	void init() override {
		rigidbody = &entity->getComponent<RigidBodyComponent>();
		animator = &entity->getComponent<AnimatorComponent>();
		moving_animator = &entity->getComponent<MovingAnimatorComponent>();
		flash_animator = &entity->getComponent<FlashAnimatorComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		transform = &entity->getComponent<TransformComponent>();
		keyboard = &entity->getComponent<KeyboardControllerComponent>();
		sword = &entity->getComponent<Sword>();

		light = &manager.addEntity(); 

		light->addComponent<TransformComponent>(transform->position.x, transform->position.y,
			600.0f, 600.0f, transform->scale);
		light->addComponent<LightComponent>(1);
		light->getComponent<LightComponent>().color = Color(255, 255, 255, 150);

		light->addGroup(Game::groupLights);

		if (!entity->hasComponent<LivingCharacter>()) //todo: problem: having transform on top left grid, not every collider its own
		{
			entity->addComponent<LivingCharacter>();
		}
		living = &entity->getComponent<LivingCharacter>();
	}

	void update(float deltaTime) override {
		if (light) {
			TransformComponent* lightTransform = &light->getComponent<TransformComponent>();
			light->getComponent<TransformComponent>().position.x = transform->position.x - lightTransform->width/2 + transform->width/2;
			light->getComponent<TransformComponent>().position.y = transform->position.y - lightTransform->height / 2 + transform->height / 2;
		}

		if (!attackAnimation) {
			if (keyboard->_inputManager.isKeyDown(keyboard->attackKey)) {
				animator->Play("P1Attack");
				_slashEffect.play();
				this->attackAnimation = true;
				this->action = Player_Script::playerAction::PLAYERACTION_ATTACK;
			}
			if (keyboard->_inputManager.isKeyDown(keyboard->ability1Key)) {
				animator->Play("P1Ability1");
				_slashSkillEffect.play();
				this->attackAnimation = true;
				this->action = Player_Script::playerAction::PLAYERACTION_ABILITY_1;
			}
		}
		else {
			if (sprite->animation.hasFinished()) {
				this->attackAnimation = false;
				this->action = Player_Script::playerAction::PLAYERACTION_IDLE;
			}
		}

		if (!vertTransitionPlayerAnimation && !horTransitionPlayerAnimation) 
		{
			if (keyboard->_inputManager.isKeyDown(keyboard->walkRightKey))
			{
				if (this->leftofPipe)
				{
					sprite->transform->velocity.x = 0;
					moving_animator->Play("PlayerHorTransition");
					this->horTransitionPlayerAnimation = true;
					this->action = Player_Script::playerAction::PLAYERACTION_JUMP;
				}
			}
			if (keyboard->_inputManager.isKeyDown(keyboard->downKey))
			{
				if (this->onPipe)
				{
					sprite->transform->velocity.x = 0;
					moving_animator->Play("PlayerVertTransition");
					this->vertTransitionPlayerAnimation = true;
					this->action = Player_Script::playerAction::PLAYERACTION_JUMP;
				}
			}
		}
		if (vertTransitionPlayerAnimation || horTransitionPlayerAnimation) // transition on pipe
		{	
			if (sprite->moving_animation.hasFinished()) {
				if (vertTransitionPlayerAnimation) {
					finishedVertAnimation = true;
					vertTransitionPlayerAnimation = false;
				}
				else {
					finishedHorAnimation = true;
					horTransitionPlayerAnimation = false;
					//map completed
					Game::map->setMapCompleted(true);
				}
			}
		}

		if (!tookDamage) {
			if (living->tookDamage) {
				flash_animator->Play("PlayerHit");
				_takeHitEffect.play();
				tookDamage = true;
			}
		}
		else {
			if (sprite->flash_animation.hasFinished()) {
				tookDamage = false;
			}
		}

		if (action == playerAction::PLAYERACTION_ATTACK) { //if playerAttackAnimation is on 3rd frame then deal damage i.e create entity from sword that deal damage.
			if (sprite->animation.cur_frame_index == 1 && sprite->animation.frame_times_played == 1)
			{
				sword->attack();
			}
			return;
		}
		else if (action == playerAction::PLAYERACTION_ABILITY_1) {
			if (sprite->animation.cur_frame_index == 1 && sprite->animation.frame_times_played == 1)
			{
				sword->ability1();
			}
			return;
		}
		else if (!rigidbody->onGround)
		{
			if (action == playerAction::PLAYERACTION_JUMP)
				return;
			action = playerAction::PLAYERACTION_JUMP;
		}
		else if (rigidbody->onGround && transform->velocity.x == 0 
			&& action != playerAction::PLAYERACTION_ATTACK && action != playerAction::PLAYERACTION_ABILITY_1)
		{
			if (action == playerAction::PLAYERACTION_IDLE)
				return;
			action = playerAction::PLAYERACTION_IDLE;
		}
		else if (rigidbody->onGround && transform->velocity.x != 0)
		{
			if (action == playerAction::PLAYERACTION_WALK)
				return;
			action = playerAction::PLAYERACTION_WALK;
		}

		switch (action)
		{
		case playerAction::PLAYERACTION_IDLE:
			animator->Play(keyboard->idleAnimation);
			break;
		case playerAction::PLAYERACTION_WALK:
			animator->Play(keyboard->walkAnimation);
			break;
		case playerAction::PLAYERACTION_RUN:
			animator->Play(keyboard->walkAnimation);
			break;
		case playerAction::PLAYERACTION_JUMP:
			animator->Play(keyboard->jumpAnimation);
			break;
		case playerAction::PLAYERACTION_ATTACK:
			animator->Play(keyboard->attackAnimation);
			break;
		default:
			break;
		}

		onPipe = false;
		leftofPipe = false;
	}

};