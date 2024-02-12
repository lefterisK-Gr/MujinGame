#pragma once

#include "../ECS.h"
#include "../Components.h"
#include "Vector2D/Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update(float deltaTime) override
	{
		sprite->srcRect.x = 0; //0
		sprite->srcRect.y = 0; //0

		distance += speed * deltaTime;

		if (distance > range)
		{
			entity->destroy();
		}
	}
private:

	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};