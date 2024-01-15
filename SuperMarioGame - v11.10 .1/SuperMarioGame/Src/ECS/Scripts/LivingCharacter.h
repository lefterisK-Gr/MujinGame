#pragma once

#include "../Components.h"

class Player_Script;
class HPBar;

#include "HPBar.h"

class LivingCharacter : public Component
{
public:
	TransformComponent* transform = nullptr;

	bool tookDamage = false;

	Entity* hp_bar;
	Entity* mana_bar;
	Entity* greyBar;

	LivingCharacter()
	{

	}

	~LivingCharacter() {
		greyBar->destroy();
		hp_bar->destroy();
		if (mana_bar) {
			mana_bar->destroy();
		}
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		hp_bar = &manager.addEntity();
		greyBar = &manager.addEntity();
		
		
		if (entity->hasComponent<Player_Script>()) {
			greyBar->addComponent<TransformComponent>(transform->position.x, transform->position.y, 10.0f, 100.0f, transform->scale);
			greyBar->addComponent<HPBar>();
			greyBar->getComponent<HPBar>().color = Color(128, 128, 128, 255); // Grey color

			hp_bar->addComponent<TransformComponent>(300, 780,
				10.0f, 100.0f, transform->scale);
			hp_bar->addComponent<HPBar>();
			hp_bar->getComponent<HPBar>().color = Color(255, 0, 0, 255);

			mana_bar = &manager.addEntity();

			mana_bar->addComponent<TransformComponent>(300, 780,
				5.0f, 100.0f, transform->scale);
			mana_bar->addComponent<HPBar>();
			mana_bar->getComponent<HPBar>().color = Color(0, 0, 255, 255);

			mana_bar->addGroup(Game::groupHPBars);
		}
		else {
			greyBar->addComponent<TransformComponent>(transform->position.x, transform->position.y, 5.0f, 50.0f, transform->scale);
			greyBar->addComponent<HPBar>();
			greyBar->getComponent<HPBar>().color = Color(128, 128, 128, 255); // Grey color

			hp_bar->addComponent<TransformComponent>(transform->position.x, transform->position.y,
				5.0f, 50.0f, transform->scale);
			hp_bar->addComponent<HPBar>();
			hp_bar->getComponent<HPBar>().color = Color(0, 255, 0, 255);
		}
		greyBar->addGroup(Game::groupHPBars);

		hp_bar->addGroup(Game::groupHPBars);
	}

	void update(float deltaTime) override {
		tookDamage = false;

		TransformComponent* defaultBarTransform = &greyBar->getComponent<TransformComponent>();
		greyBar->getComponent<TransformComponent>().position.x = transform->position.x - defaultBarTransform->width / 2 + transform->width / 2;
		greyBar->getComponent<TransformComponent>().position.y = transform->position.y;

		if (hp_bar) {
			TransformComponent* hpBarTransform = &hp_bar->getComponent<TransformComponent>();
			hp_bar->getComponent<TransformComponent>().position.x = transform->position.x - hpBarTransform->width / 2 + transform->width / 2;
			hp_bar->getComponent<TransformComponent>().position.y = transform->position.y;
		}
		if (mana_bar) {
			TransformComponent* manaBarTransform = &mana_bar->getComponent<TransformComponent>();
			mana_bar->getComponent<TransformComponent>().position.x = transform->position.x - manaBarTransform->width / 2 + transform->width / 2;
			mana_bar->getComponent<TransformComponent>().position.y = transform->position.y + 10;
		
		}
	}

	void draw() override {
		//glm::vec4 pos((float)destRect.x, -640 + (float)destRect.y, (float)destRect.w, (float)destRect.h);
		//Game::_spriteBatch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color);
	}

	bool applyDamage(float damage) {
		tookDamage = true;
		hp_bar->getComponent<HPBar>()._healthPoints -= damage;
		// If we died, return true
		if (hp_bar->getComponent<HPBar>()._healthPoints <= 0) {
			return true;
		}
		return false;
	}

	bool applyMana(float mana) {
		if (mana_bar) {
			if (mana_bar->getComponent<HPBar>()._healthPoints <= 0) {
				return true;
			}
			mana_bar->getComponent<HPBar>()._healthPoints -= mana;
			// If we died, return true
		}
		return false;
	}

};