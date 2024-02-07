#pragma once
#include <string>
#include <SDL/SDL.h>
#include "Components.h"
#include "../TextureManager.h"

#define COL_POS_OFFSET 8

class ColliderComponent : public Component //collider -> transform
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Rect srcR, destR;

	TransformComponent* transform = nullptr;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	ColliderComponent(std::string t, SDL_Rect colliderRect)
	{
		tag = t;
		collider = colliderRect;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>()) //todo: problem: having transform on top left grid, not every collider its own
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		if (tag == "terrain")
		{
		}
		srcR = { 0, 0, 8, 8 };
		destR = { collider.x, collider.y , collider.w , collider.h };

		//Game::colliders.push_back(this);

	}

	void update(float deltaTime) override
	{
		if (tag != "terrain") // for all npcs
		{
			collider.x = static_cast<int>(transform->position.x) + (2 * (transform->scale) * COL_POS_OFFSET);
			collider.y = static_cast<int>(transform->position.y) + (2 * (transform->scale) * COL_POS_OFFSET);
			collider.w = (transform->width * transform->scale) - (4 * (transform->scale) * COL_POS_OFFSET);
			collider.h = (transform->height * transform->scale) - (2 * (transform->scale) * COL_POS_OFFSET);
		}
		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw(SpriteBatch& batch) override
	{
	}

	SDL_Rect getRect() override
	{
		return collider;
	}
};