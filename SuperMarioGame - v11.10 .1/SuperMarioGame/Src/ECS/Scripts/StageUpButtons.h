#pragma once

#include "../Components.h"
#include "../../AudioEngine/AudioEngine.h"

class StageUpButtons : public Component
{
private:
	std::vector<Entity*> _stageupbtns;
public:
	bool isHud = false;

	TransformComponent* transform = nullptr;

	Entity* attackDamageBtn;
	Entity* defenceBtn;
	Entity* hpPotionBtn;

	StageUpButtons()
	{

	}

	StageUpButtons(bool is_hud)
	{
		isHud = is_hud;
	}

	~StageUpButtons() {
		for (auto& s : _stageupbtns)
		{
			s->destroy();
		}
		_stageupbtns.clear();
		// Assuming manager.getGroup returns a reference to the group
		auto& stageupbtns = manager.getGroup(Game::groupStageUpButtons);
		auto& stageupbtnsback = manager.getGroup(Game::groupStageUpButtonsBack);

		// Clear the entities from the groups
		stageupbtns.clear();
		stageupbtnsback.clear();
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		auto& attackDamageBtn(manager.addEntity());
		auto& defenceBtn(manager.addEntity());
		auto& hpPotionBtn(manager.addEntity());

		attackDamageBtn.addComponent<StageUpButton>("projectile", 0.0f, isHud);
		attackDamageBtn.addComponent<ButtonComponent>(std::bind(&StageUpButtons::handleAttackDamageBtn, this));

		defenceBtn.addComponent<StageUpButton>("projectile", 100.0f, isHud);
		defenceBtn.addComponent<ButtonComponent>(std::bind(&StageUpButtons::handleDefenceBtn, this));

		hpPotionBtn.addComponent<StageUpButton>("projectile", 200.0f, isHud);
		hpPotionBtn.addComponent<ButtonComponent>(std::bind(&StageUpButtons::handleHpPotionBtn, this));

		_stageupbtns.push_back(&attackDamageBtn);
		_stageupbtns.push_back(&defenceBtn);
		_stageupbtns.push_back(&hpPotionBtn);
	}

	void update(float deltaTime) override {
	}

	void handleAttackDamageBtn() {
		auto& players(manager.getGroup(Game::groupPlayers));

		for (auto& p : players)
		{
			p->getComponent<Sword>().levelUpAttackDamage(10);
		}
		entity->destroy();
	}

	void handleDefenceBtn() {
		auto& players(manager.getGroup(Game::groupPlayers));

		for (auto& p : players)
		{
			p->getComponent<LivingCharacter>().defence += 10;
		}
		entity->destroy();
	}

	void handleHpPotionBtn() {
		auto& players(manager.getGroup(Game::groupPlayers));

		for (auto& p : players)
		{
			p->getComponent<LivingCharacter>().hp_bar->getComponent<HPBar>()._healthPoints += 10;
		}
		entity->destroy();
	}

};