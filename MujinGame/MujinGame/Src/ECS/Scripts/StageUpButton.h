#pragma once

#include "ECS/Components.h"
#include "AudioEngine/AudioEngine.h"

class StageUpButton : public Component
{
public:
	std::string idName;

	float offsetX = 0;
	float offsetY = 0;
	bool isHud = false;

	TransformComponent* transform = nullptr;

	Entity* stageUpBtnBack;

	StageUpButton()
	{

	}

	StageUpButton(std::string textId, float offset_x, float offset_y, bool is_hud)
	{
		idName = textId;
		offsetX = offset_x;
		offsetY = offset_y;
		isHud = is_hud;
	}

	~StageUpButton() {
		stageUpBtnBack->destroy();
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(glm::vec2(0.0f, offsetY),
				glm::ivec2(
					TextureManager::getInstance().Get_GLTexture(idName)->width,
					TextureManager::getInstance().Get_GLTexture(idName)->height
				),
				1.0f);
		}
		transform = &entity->GetComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(idName);
		}
		entity->addGroup(Manager::groupStageUpButtons);

		stageUpBtnBack = &manager.addEntity();
		stageUpBtnBack->addComponent<TransformComponent>(transform->getPosition(), glm::ivec2( 32, 32), transform->scale);
		stageUpBtnBack->addComponent<Rectangle_w_Color>();
		stageUpBtnBack->GetComponent<Rectangle_w_Color>().color = Color(128, 128, 128, 255); // Grey color

		stageUpBtnBack->addGroup(Manager::groupStageUpButtonsBack);
	}

	void update(float deltaTime) override {
		if (isHud) {
			entity->GetComponent<TransformComponent>().setPosition_X( offsetX);
			stageUpBtnBack->GetComponent<TransformComponent>().setPosition_X(offsetX);
		}
	}

};