#pragma once

#include "ECS/Components.h"
#include "../../Map/Map.h"
#include "AudioEngine/AudioEngine.h"
#include "../../Collision/Collision.h"
#include "../Scripts/Shop.h"
#include "../Scripts/Inventory.h"

class Shop;

class Player_Script : public Component //PlayerAnimator -> Animator -> Sprite -> Transform
{
private:
	SoundEffect _slashEffect = Game::audioEngine.loadSoundEffect("Sounds/slash.wav");
	SoundEffect _slashSkillEffect = Game::audioEngine.loadSoundEffect("Sounds/skillSlash.wav");
	SoundEffect _takeHitEffect = Game::audioEngine.loadSoundEffect("Sounds/takeHit.wav");

	float _zIndex = 1.0f;

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
		rigidbody = &entity->GetComponent<RigidBodyComponent>();
		animator = &entity->GetComponent<AnimatorComponent>();
		moving_animator = &entity->GetComponent<MovingAnimatorComponent>();
		flash_animator = &entity->GetComponent<FlashAnimatorComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
		transform = &entity->GetComponent<TransformComponent>();
		keyboard = &entity->GetComponent<KeyboardControllerComponent>();
		sword = &entity->GetComponent<Sword>();

		light = &manager.addEntity(); 

		light->addComponent<TransformComponent>(transform->position.x, transform->position.y);
		light->addComponent<LightComponent>(1);
		light->GetComponent<LightComponent>().color = Color(255, 255, 200, 150);

		light->addGroup(Game::groupLights);

		if (!entity->hasComponent<LivingCharacter>()) //todo: problem: having transform on top left grid, not every collider its own
		{
			entity->addComponent<LivingCharacter>();
		}
		living = &entity->GetComponent<LivingCharacter>();
	}

	void update(float deltaTime) override {
		float parallaxFactor = 1.0f / _zIndex;
		if (light) {
			TransformComponent* lightTransform = &light->GetComponent<TransformComponent>();
			light->GetComponent<TransformComponent>().position.x = transform->position.x + transform->width/2 - (Game::camera.x * parallaxFactor);
			light->GetComponent<TransformComponent>().position.y = transform->position.y + transform->height/2 - (Game::camera.y * parallaxFactor);
		}

		if (!attackAnimation) {
			if (keyboard->_inputManager.isKeyDown(keyboard->attackKey) && !living->exhaust(10)) {
				animator->Play("P1Attack");
				_slashEffect.play();
				this->attackAnimation = true;
				this->action = Player_Script::playerAction::PLAYERACTION_ATTACK;
			}
			if (keyboard->_inputManager.isKeyDown(keyboard->ability1Key) && !living->exhaust(10)) {
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

		if (	keyboard->_inputManager.isKeyDown(keyboard->runKey) 
			&&	(	keyboard->_inputManager.isKeyDown(keyboard->walkRightKey) || keyboard->_inputManager.isKeyDown(keyboard->walkLeftKey)	)) {
			if (!living->exhaust(0.2f)) {
				transform->velocity.x *= 1.2;
			}
		}
		else {
			living->recover(0.1);
		}

		if (keyboard->_inputManager.isKeyPressed(keyboard->pickUpKey))
		{
			auto& markettiles(manager.getGroup(Game::groupMarket));

			for (auto& mt : markettiles) {
				if (Collision::checkCollision(mt->GetComponent<ColliderComponent>().collider, entity->GetComponent<ColliderComponent>().collider)) {
					// ****OPEN SHOP****
					auto& shops(manager.getGroup(Game::groupShops));
					for (auto& sh : shops)
					{
						sh->GetComponent<Shop>().isOpen = !sh->GetComponent<Shop>().isOpen;
					}
					break;
				}
			}
		}

		if (keyboard->_inputManager.isKeyPressed(keyboard->inventoryKey))
		{
			auto& inventories(manager.getGroup(Game::groupInventories));
			for (auto& sh : inventories)
			{
				sh->GetComponent<Inventory>().isOpen = !sh->GetComponent<Inventory>().isOpen;
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