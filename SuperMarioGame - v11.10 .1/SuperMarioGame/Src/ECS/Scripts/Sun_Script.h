#pragma once

#include "../Components.h"
#include "../../ShapeManager.h"

class Sun_Script : public Component
{
public:
	TransformComponent* transform;

	SDL_Color color = { static_cast < uint8_t>(255),  static_cast <uint8_t>(255),  static_cast <uint8_t>(255),  static_cast <uint8_t>(0.5) };

	Sun_Script()
	{

	}

	Sun_Script(SDL_Color col)
	{
		color = col;
	}

	~Sun_Script() {

	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {

	}

	void draw() override {
		ShapeManager::DrawCircle(transform->position.x, transform->position.y, transform->width / 2, color);
	}

};