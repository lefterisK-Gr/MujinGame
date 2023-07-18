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
	
	}

};