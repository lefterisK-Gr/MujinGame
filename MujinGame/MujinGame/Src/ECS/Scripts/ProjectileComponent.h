#pragma once

#include "ECS/ECS.h"
#include "ECS/Components.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, glm::ivec2 vel) : range(rng), speed(sp), velocity(vel)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->GetComponent<TransformComponent>();
		transform->setVelocity_X(velocity.x);
		transform->setVelocity_Y(velocity.y);
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
	glm::vec2 velocity;
};