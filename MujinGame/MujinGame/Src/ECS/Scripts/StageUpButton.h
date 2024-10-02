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
			entity->addComponent<TransformComponent>(0.0f, offsetY,
				TextureManager::getInstance().Get_GLTexture(idName)->height,
				TextureManager::getInstance().Get_GLTexture(idName)->width,
				1.0f);
		}
		transform = &entity->GetComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(idName, 1.0f);
		}
		entity->addGroup(Manager::groupStageUpButtons);

		stageUpBtnBack = &manager.addEntity();
		stageUpBtnBack->addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y, 32.0f, 32.0f, transform->scale);
		stageUpBtnBack->addComponent<Rectangle_w_Color>();
		stageUpBtnBack->GetComponent<Rectangle_w_Color>().color = Color(128, 128, 128, 255); // Grey color

		stageUpBtnBack->addGroup(Manager::groupStageUpButtonsBack);
	}

	void update(float deltaTime) override {
		if (isHud) {
			std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

			entity->GetComponent<TransformComponent>().setPosition_X(main_camera2D->worldLocation.x + offsetX);
			stageUpBtnBack->GetComponent<TransformComponent>().setPosition_X(main_camera2D->worldLocation.x + offsetX);
		}
	}

};