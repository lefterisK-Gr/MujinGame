#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"
#include "../TextureManager.h"

#define COL_POS_OFFSET 8

class ColliderComponent : public Component //collider -> transform
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	bool hasColliderAbove = false;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size , bool hascolab)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
		hasColliderAbove = hascolab;
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
			tex = TextureManager::LoadTexture("assets/grid_collision.png");
		}
		srcR = { 0, 0, 8, 8 };
		destR = { collider.x, collider.y , collider.w , collider.h };

		//Game::colliders.push_back(this);

	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x) + COL_POS_OFFSET;
			collider.y = static_cast<int>(transform->position.y) + COL_POS_OFFSET;
			collider.w = (transform->width * transform->scale) - (2 * COL_POS_OFFSET);
			collider.h = (transform->height * transform->scale) - COL_POS_OFFSET;
		}
		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE); //remove comment to add collider boxes
	}

	SDL_Rect getRect() override
	{
		return collider;
	}

	bool getHasGridAbove() override
	{
		return hasColliderAbove;
	}
};