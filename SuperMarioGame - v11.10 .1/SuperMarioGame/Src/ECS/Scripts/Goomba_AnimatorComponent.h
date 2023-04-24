#pragma once

#include "../AnimatorComponent.h"

class Goomba_AnimatorComponent : public AnimatorComponent
{
public:
	bool shelltoturtle = false;

	Goomba_AnimatorComponent()
		: AnimatorComponent("goomba", true, false)
	{

	}

	~Goomba_AnimatorComponent() {

	}

	void update() override {
		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height;

		sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;
		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale;
	}
};