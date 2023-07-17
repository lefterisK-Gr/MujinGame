#pragma once

#include "../Components.h"

class Goomba_Script : public Component
{
public:
	SpriteComponent* sprite;

	Goomba_Script()
	{

	}

	~Goomba_Script() {

	}

	void init() override {
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height;
	}

};