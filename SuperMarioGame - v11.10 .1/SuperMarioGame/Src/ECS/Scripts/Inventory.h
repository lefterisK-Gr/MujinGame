#pragma once

#include "../Components.h"

class Inventory : public Component
{
public:

	bool isOpen = false;

	TransformComponent* transform = nullptr;
	Rectangle_w_Color* rectangle = nullptr;

	Inventory()
	{

	}

	~Inventory() {

	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(50.0f, 50.0f, 400.0f, 600.0f, 1);
		}
		transform = &entity->getComponent<TransformComponent>();
		if (!entity->hasComponent<Rectangle_w_Color>()) {
			entity->addComponent<Rectangle_w_Color>();
		}
		rectangle = &entity->getComponent<Rectangle_w_Color>();
		rectangle->color = Color(200, 200, 200, 255);
	}

	void update(float deltaTime) override {
		transform->position.x = isOpen ? Game::camera.x + 50.0f : -1000.0f;
	}

	void draw(SpriteBatch& batch) override {
	}
};