#pragma once

#include <string>
#include <SDL/SDL.h>
#include "Components.h"

class TileComponent : public Component //tileComp --> GridComp --> ColliderComp -> transform
{
public:
	bool fullSolid;
	bool hasGrid = false;

	std::string textureid;

	SDL_Rect srcRect, destRect;
	glm::ivec2 position;

	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;
	MovingAnimatorComponent* moving_animator;
	GridComponent* grid;
	int scaledTile;

	TileComponent() = default;

	~TileComponent()
	{
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos,int tsize, int tscale, std::string id , bool isSolid )
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

		scaledTile = tsize * tscale;
	}
	
	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>(static_cast<glm::vec2>(position) , glm::ivec2(32, 32), 1);
			entity->GetComponent<TransformComponent>().setVelocity_X(0);
			entity->GetComponent<TransformComponent>().setVelocity_Y(0);
		}
		transform = &entity->GetComponent<TransformComponent>();

		if (!entity->hasComponent<SpriteComponent>())
		{
			entity->addComponent<SpriteComponent>(textureid);
		}
		sprite = &entity->GetComponent<SpriteComponent>();
		sprite->srcRect = this->srcRect;
		sprite->destRect = this->destRect;

		if (!entity->hasComponent<GridComponent>() && fullSolid)
		{
			entity->addComponent<GridComponent>(position.x, position.y, scaledTile);
		}
		grid = &entity->GetComponent<GridComponent>();
	}
	
	void update(float deltaTime) override //function like in sprite, but diffent moving in draw
	{
		
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override
	{
		//sprite->draw(batch);
		//TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};