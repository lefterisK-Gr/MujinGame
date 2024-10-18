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


	}

	void update(float deltaTime) override {
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
	}
};