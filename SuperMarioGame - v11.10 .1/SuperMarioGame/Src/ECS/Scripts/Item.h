#pragma once

#include "../Components.h"

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
		itemBack = &manager.addEntity();
		itemBack->addComponent<TransformComponent>(transform->position.x, transform->position.y, 32.0f, 32.0f, transform->scale);
		itemBack->addComponent<Rectangle_w_Color>();
		itemBack->getComponent<Rectangle_w_Color>().color = Color(128, 128, 128, 255); // Grey color

		itemBack->addGroup(Game::groupStageUpButtonsBack);
	}

	void update(float deltaTime) override {

	}

	void draw(SpriteBatch& batch) override {
	}
};