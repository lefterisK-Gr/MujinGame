#pragma once

#include "../Components.h"
#include "../../AudioEngine/AudioEngine.h"

class StageUpButton : public Component
{
public:
	std::string id;

	float offsetX = 0;
	bool isHud = false;

	TransformComponent* transform = nullptr;

	Entity* stageUpBtnBack;

	StageUpButton()
	{

	}

	StageUpButton(std::string textId, float offset_x, bool is_hud)
	{
		id = textId;
		offsetX = offset_x;
		isHud = is_hud;
	}

	~StageUpButton() {
		stageUpBtnBack->destroy();
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0.0f, 100.0f,
				Game::assets->Get_GLTexture(id)->height,
				Game::assets->Get_GLTexture(id)->width,
				1.0f);
		}
		transform = &entity->getComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(id, 1.0f);
		}
		entity->addGroup(Game::groupStageUpButtons);

		stageUpBtnBack = &manager.addEntity();
		stageUpBtnBack->addComponent<TransformComponent>(transform->position.x, transform->position.y, 32.0f, 32.0f, transform->scale);
		stageUpBtnBack->addComponent<HPBar>();
		stageUpBtnBack->getComponent<HPBar>().color = Color(128, 128, 128, 255); // Grey color

		stageUpBtnBack->addGroup(Game::groupStageUpButtonsBack);
	}

	void update(float deltaTime) override {
		if (isHud) {
			entity->getComponent<TransformComponent>().position.x = Game::camera.x + offsetX;
			stageUpBtnBack->getComponent<TransformComponent>().position.x = Game::camera.x + offsetX;
		}
	}

};