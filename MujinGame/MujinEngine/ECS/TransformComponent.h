#pragma once

#include "Components.h"
#include "Vector2D/Vector2D.h"

class TransformComponent : public Component //transform as in graphics, we have rotation and scale
{
public:

	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 1;

	bool activatedMovement = false;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int sc)
	{
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	TransformComponent(float x, float y, int h, int w, int sc, int sp)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
		speed = sp;
	}
	
	void init() override
	{
		velocity.Zero();
	}
	void update(float deltaTime) override
	{
		position.x += velocity.x * speed * deltaTime;
		position.y += velocity.y * speed;
	}

	Vector2D getCenterTransform()
	{
		return Vector2D(position.x + width * scale / 2, position.y + height * scale / 2);
	}
};