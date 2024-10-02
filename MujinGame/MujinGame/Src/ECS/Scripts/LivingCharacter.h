#pragma once

#include "ECS/Components.h"

class Player_Script;
class HPBar;

#include "HPBar.h"

class LivingCharacter : public Component
{
public:
	TransformComponent* transform = nullptr;

	int defence = 0;
	bool tookDamage = false;
	bool isActive = false;
	Entity* hp_bar;
	Entity* mana_bar; // todo: dont make all entities have mana bar, takes space
	Entity* stamina_bar; // todo: dont make all entities have stamina bar, takes space
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
		if (stamina_bar) {
			stamina_bar->destroy();
		}
	}

	void init() override {
		transform = &entity->GetComponent<TransformComponent>();

		hp_bar = &manager.addEntity();
		greyBar = &manager.addEntity();
		
		
		if (entity->hasComponent<Player_Script>()) {
			greyBar->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y - 10.0f, 10.0f, 100.0f, transform->scale);
			greyBar->addComponent<HPBar>();
			greyBar->GetComponent<HPBar>().rectangle->color= Color(128, 128, 128, 255); // Grey color
			greyBar->addGroup(Manager::groupHPBars);

			hp_bar->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y - 10.0f,
				10.0f, 100.0f, transform->scale);
			hp_bar->addComponent<HPBar>();
			hp_bar->GetComponent<HPBar>().rectangle->color = Color(255, 0, 0, 255);
			hp_bar->addGroup(Manager::groupHPBars);

			mana_bar = &manager.addEntity();

			mana_bar->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y,
				5.0f, 100.0f, transform->scale);
			mana_bar->addComponent<HPBar>();
			mana_bar->GetComponent<HPBar>().rectangle->color = Color(0, 0, 255, 255);

			mana_bar->addGroup(Manager::groupHPBars);

			stamina_bar = &manager.addEntity();

			stamina_bar->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y + 5,
				5.0f, 100.0f, transform->scale);
			stamina_bar->addComponent<HPBar>();
			stamina_bar->GetComponent<HPBar>().rectangle->color = Color(0, 100, 0, 255);

			stamina_bar->addGroup(Manager::groupHPBars);
		}
		else {
			greyBar->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y, 5.0f, 50.0f, transform->scale);
			greyBar->addComponent<HPBar>();
			greyBar->GetComponent<HPBar>().rectangle->color = Color(128, 128, 128, 255); // Grey color
			greyBar->addGroup(Manager::groupHPBars);
			hp_bar->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y,
				5.0f, 50.0f, transform->scale);
			hp_bar->addComponent<HPBar>();
			hp_bar->GetComponent<HPBar>().rectangle->color = Color(0, 255, 0, 255);
			hp_bar->addGroup(Manager::groupHPBars);
		}
	}

	void update(float deltaTime) override {
		tookDamage = false;

		TransformComponent* defaultBarTransform = &greyBar->GetComponent<TransformComponent>();
		greyBar->GetComponent<TransformComponent>().setPosition_X(transform->getPosition().x - defaultBarTransform->width / 2 + transform->width / 2);
		greyBar->GetComponent<TransformComponent>().setPosition_Y(transform->getPosition().y - 10);

		if (hp_bar) {
			TransformComponent* hpBarTransform = &hp_bar->GetComponent<TransformComponent>();
			hp_bar->GetComponent<TransformComponent>().setPosition_X(transform->getPosition().x - hpBarTransform->width / 2 + transform->width / 2);
			hp_bar->GetComponent<TransformComponent>().setPosition_Y(transform->getPosition().y - 10);
		}
		if (mana_bar) {
			TransformComponent* manaBarTransform = &mana_bar->GetComponent<TransformComponent>();
			mana_bar->GetComponent<TransformComponent>().setPosition_X(transform->getPosition().x - manaBarTransform->width / 2 + transform->width / 2);
			mana_bar->GetComponent<TransformComponent>().setPosition_Y(transform->getPosition().y);
		
		}
		if (stamina_bar) {
			TransformComponent* staminaBarTransform = &stamina_bar->GetComponent<TransformComponent>();
			stamina_bar->GetComponent<TransformComponent>().setPosition_X (transform->getPosition().x - staminaBarTransform->width / 2 + transform->width / 2);
			stamina_bar->GetComponent<TransformComponent>().setPosition_Y (transform->getPosition().y + 5);
		
		}
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
		//glm::vec4 pos((float)destRect.x, -640 + (float)destRect.y, (float)destRect.w, (float)destRect.h);
		//Game::_spriteBatch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color);
	}

	bool applyDamage(float damage) {
		tookDamage = true;
		hp_bar->GetComponent<HPBar>()._healthPoints -= damage * 100 / (100 + defence) ;
		// If we died, return true
		if (hp_bar->GetComponent<HPBar>()._healthPoints <= 0) {
			return true;
		}
		return false;
	}

	bool applyMana(float mana) {
		if (mana_bar) {
			if (mana_bar->GetComponent<HPBar>()._healthPoints <= 0) {
				return true;
			}
			mana_bar->GetComponent<HPBar>()._healthPoints -= mana;
			// If we died, return true
		}
		return false;
	}

	bool exhaust(float stamina) {
		if (stamina_bar) {
			if (stamina_bar->GetComponent<HPBar>()._healthPoints - stamina <= 0) {
				return true;
			}
			stamina_bar->GetComponent<HPBar>()._healthPoints -= stamina;
		}
		return false;
	}

	bool recover(float stamina) {
		if (stamina_bar) {
			if (stamina_bar->GetComponent<HPBar>()._healthPoints >= 100) {
				return true;
			}
			stamina_bar->GetComponent<HPBar>()._healthPoints += stamina;
		}
		return false;
	}

};