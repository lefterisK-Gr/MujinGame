#pragma once

#include "ECS/Components.h"

class Item : public Component
{
public:

	std::string id;

	TransformComponent* transform = nullptr;

	Entity* itemBack;

	float price;

	Item()
	{

	}

	Item(std::string textId, float _price)
	{
		id = textId;
		price = _price;
	}

	~Item() {
		itemBack->destroy();
	}

	void init() override {

		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0.0f, 100.0f,
				TextureManager::getInstance().Get_GLTexture(id)->height,
				TextureManager::getInstance().Get_GLTexture(id)->width,
				2.0f);
		}
		transform = &entity->getComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>()) {
			entity->addComponent<SpriteComponent>(id, 1.0f);
		}
		entity->addGroup(Game::groupStageUpButtons);

		itemBack = &manager.addEntity();
		itemBack->addComponent<TransformComponent>(transform->position.x, transform->position.y, 32.0f, 32.0f, transform->scale);
		itemBack->addComponent<Rectangle_w_Color>();
		itemBack->getComponent<Rectangle_w_Color>().color = Color(255, 0, 0, 255); // Grey color

		itemBack->addGroup(Game::groupStageUpButtonsBack);
	}

	void update(float deltaTime) override {
		itemBack->getComponent<TransformComponent>().position.x = transform->position.x;
	}

	void draw(SpriteBatch& batch) override {
	}
};