#pragma once

#include "../ECS.h"
#include "../Components.h"
#include "../../Vector2D.h"

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

	void update() override
	{
		sprite->srcRect.x = 0; //0
		sprite->srcRect.y = 0; //0

		distance += speed;

		if (distance > range)
		{
			std::cout << "Out of Range" << std::endl; //range and bounds are different
			entity->destroy();
		}
		/*else if (transform->position.x > Game::camera.x + Game::camera.w ||
				transform->position.x < Game::camera.x ||
				transform->position.y > Game::camera.y + Game::camera.h ||
				transform->position.y < Game::camera.y)
		{
			std::cout << "Out of bounds!" << std::endl; //stop rendering it if it out of bounds, otherwise program slow for no reasson
			entity->destroy();
		}*/
	}
private:

	TransformComponent* transform;
	SpriteComponent* sprite;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};