#pragma once

#include "../Components.h"

class Player_Script;

class LivingCharacter : public Component
{
public:
	TransformComponent* transform = nullptr;

	bool tookDamage = false;

	Entity* hp_bar;

	LivingCharacter()
	{

	}

	~LivingCharacter() {
		hp_bar->destroy();
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		hp_bar = &manager.addEntity();

		if (entity->hasComponent<Player_Script>()) {
			hp_bar->addComponent<TransformComponent>(300, 780,
				10.0f, 100.0f, transform->scale);
			hp_bar->addComponent<HPBar>();
			hp_bar->getComponent<HPBar>().color = Color(255, 0, 0, 255);
		}
		else {
			hp_bar->addComponent<TransformComponent>(transform->position.x, transform->position.y,
				5.0f, 50.0f, transform->scale);
			hp_bar->addComponent<HPBar>();
			hp_bar->getComponent<HPBar>().color = Color(0, 255, 0, 255);
		}
		hp_bar->addGroup(Game::groupHPBars);
	}

	void update(float deltaTime) override {
		tookDamage = false;
		if (hp_bar) {
			TransformComponent* hpBarTransform = &hp_bar->getComponent<TransformComponent>();
			hp_bar->getComponent<TransformComponent>().position.x = transform->position.x - hpBarTransform->width / 2 + transform->width / 2;
			hp_bar->getComponent<TransformComponent>().position.y = transform->position.y;
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

};