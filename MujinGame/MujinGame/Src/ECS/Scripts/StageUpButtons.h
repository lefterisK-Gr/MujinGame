#pragma once

#include "ECS/Components.h"
#include "AudioEngine/AudioEngine.h"

class StageUpButtons : public Component
{
private:
	std::vector<Entity*> _stageupbtns;
public:

	TransformComponent* transform = nullptr;

	Entity* attackDamageBtn;
	Entity* defenceBtn;
	Entity* hpPotionBtn;

	StageUpButtons()
	{

	}

	~StageUpButtons() {
		for (auto& s : _stageupbtns)
		{
			s->destroy();
		}
		_stageupbtns.clear();
		// Assuming manager.getGroup returns a reference to the group
		auto& stageupbtns = manager.getGroup(Manager::groupStageUpButtons);
		auto& stageupbtnsback = manager.getGroup(Manager::groupStageUpButtonsBack);

		// Clear the entities from the groups
		stageupbtns.clear();
		stageupbtnsback.clear();
	}

	void init() override {
		transform = &entity->GetComponent<TransformComponent>();

		auto& attackDamageBtn(manager.addEntity(true));
		auto& defenceBtn(manager.addEntity(true));
		auto& hpPotionBtn(manager.addEntity(true));

		attackDamageBtn.addComponent<StageUpButton>("sword", transform->getPosition().x + 0.0f, transform->getPosition().y);
		attackDamageBtn.addComponent<ButtonComponent>(std::bind(&StageUpButtons::handleAttackDamageBtn, this));
		attackDamageBtn.addGroup(Manager::groupStageUpAttackButtons);

		defenceBtn.addComponent<StageUpButton>("shield", transform->getPosition().x + 100.0f, transform->getPosition().y);
		defenceBtn.addComponent<ButtonComponent>(std::bind(&StageUpButtons::handleDefenceBtn, this));
		defenceBtn.addGroup(Manager::groupStageUpDefenceButtons);

		hpPotionBtn.addComponent<StageUpButton>("healthPotion", transform->getPosition().x + 200.0f, transform->getPosition().y);
		hpPotionBtn.addComponent<ButtonComponent>(std::bind(&StageUpButtons::handleHpPotionBtn, this));
		hpPotionBtn.addGroup(Manager::groupStageUpHpButtons);

		_stageupbtns.push_back(&attackDamageBtn);
		_stageupbtns.push_back(&defenceBtn);
		_stageupbtns.push_back(&hpPotionBtn);
	}

	void update(float deltaTime) override {
	}

	void handleAttackDamageBtn() {
		auto& players(manager.getGroup(Manager::groupPlayers));

		for (auto& p : players)
		{
			p->GetComponent<Sword>().levelUpAttackDamage(10);
		}
		entity->destroy();
	}

	void handleDefenceBtn() {
		auto& players(manager.getGroup(Manager::groupPlayers));

		for (auto& p : players)
		{
			p->GetComponent<LivingCharacter>().defence += 10;
		}
		entity->destroy();
	}

	void handleHpPotionBtn() {
		auto& players(manager.getGroup(Manager::groupPlayers));

		for (auto& p : players)
		{
			p->GetComponent<LivingCharacter>().hp_bar->GetComponent<HPBar>()._healthPoints += 10;
		}
		entity->destroy();
	}

};