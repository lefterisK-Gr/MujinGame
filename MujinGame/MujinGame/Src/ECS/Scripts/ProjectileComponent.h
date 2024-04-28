#pragma once

#include "ECS/ECS.h"
#include "ECS/Components.h"
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
		transform = &entity->GetComponent<TransformComponent>();
		transform->velocity = velocity;
		sprite = &entity->GetComponent<SpriteComponent>();
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