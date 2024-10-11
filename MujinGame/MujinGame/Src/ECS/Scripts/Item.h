#pragma once

#include "ECS/Components.h"

class Item : public Component
{
public:

	std::string idName;

	TransformComponent* transform = nullptr;

	Entity* itemBack;

	float price;

	Item()
	{

	}

	Item(std::string textId, float _price)
	{
		idName = textId;
		price = _price;
	}

	~Item() {
		itemBack->destroy();
	}

	void init() override {

		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(glm::vec2(0.0f, 100.0f),
				glm::ivec2(
				TextureManager::getInstance().Get_GLTexture(idName)->height,
				TextureManager::getInstance().Get_GLTexture(idName)->width
				),
				2.0f);
		}
		transform = &entity->GetComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(idName);
		}
		entity->addGroup(Manager::groupStageUpButtons);

		itemBack = &manager.addEntity();
		itemBack->addComponent<TransformComponent>(transform->getPosition(), glm::ivec2(32, 32), transform->scale);
		itemBack->addComponent<Rectangle_w_Color>();
		itemBack->GetComponent<Rectangle_w_Color>().color = Color(255, 0, 0, 255); // Grey color

		itemBack->addGroup(Manager::groupStageUpButtonsBack);
	}

	void update(float deltaTime) override {
		itemBack->GetComponent<TransformComponent>().setPosition_X(transform->getPosition().x);
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
	}
};