#pragma once

#include <string>
#include "ECS.h"
#include "SDL.h"
#include "Components.h"

extern Manager manager;

class TileComponent : public Component //tileComp --> GridComp --> ColliderComp -> transform
{
public:
	bool fullSolid;
	bool hasGrid = false;
	bool isHorizon = false;

	std::string textureid;

	SDL_Rect srcRect, destRect;
	Vector2D position;

	TransformComponent* transform;
	SpriteComponent* sprite;
	MovingAnimatorComponent* moving_animator;
	GridComponent* grid;
	int scaledTile;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(Game::assets->GetTexture(textureid));
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos,int tsize, int tscale, std::string id , bool isSolid , bool hasgrid , bool ishor)
	{
		textureid = id;

		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize; //tsize = 32

		destRect.x = xpos; //destRect. x/y may not be needed
		destRect.y = ypos;
		destRect.w = destRect.h = tsize * tscale; //tsize * tscale = 32

		fullSolid = isSolid; //todo remove this (have instead an array that shows the indexes in grid where its solid)

		hasGrid = hasgrid; //todo remove this (all have grid but not all are solid)

		isHorizon = ishor; //todo remove this (use z-order)

		scaledTile = tsize * tscale;
	}
	
	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>(position.x, position.y, 32, 32, 1);
		}
		transform = &entity->getComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>())
		{
			entity->addComponent<SpriteComponent>(textureid);
		}
		sprite = &entity->getComponent<SpriteComponent>();
		sprite->srcRect = this->srcRect;
		sprite->destRect = this->destRect;

		if (hasGrid)
		{
			if (!entity->hasComponent<GridComponent>())
			{
				entity->addComponent<GridComponent>(position.x, position.y, scaledTile, fullSolid);
			}
			grid = &entity->getComponent<GridComponent>();
		}
	}
	
	void update() override //function like in sprite, but diffent moving in draw
	{
		if (isHorizon)
		{
			sprite->destRect.x = transform->position.x - (Game::camera.x / 2); //reminding that destRect is absolutely on the screen that we see
			sprite->destRect.y = transform->position.y - Game::camera.y;
		}
		else
		{
			sprite->destRect.x = transform->position.x - Game::camera.x; //reminding that destRect is absolutely on the screen that we see
			sprite->destRect.y = transform->position.y - Game::camera.y;
		}
		
	}

	void draw() override
	{
		sprite->draw();
		//TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};