#pragma once
#include "Components.h"

class RigidBodyComponent : public Component
{
private:
	TransformComponent* transform;
public:
	float GravityForce = 1.0f;
	float accelGravity = 0.045f;
	float maxGravity = 5.f;
	bool onGround = false;
	bool justjumped = false;
	
	RigidBodyComponent() = default;

	RigidBodyComponent(float acc, float maxg)
	{
		accelGravity = acc;
		maxGravity = maxg;
	}

	~RigidBodyComponent()
	{

	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (onGround && !justjumped)
		{
			GravityForce = 1.0f;
			transform->velocity.y = static_cast<int>(GravityForce);
			GravityForce = 0.0f;
		}
		else
		{
			justjumped = false;
			GravityForce += accelGravity;
			transform->velocity.y += static_cast<int>(GravityForce);
			if (transform->velocity.y > static_cast<int>(maxGravity))
			{
				transform->velocity.y = static_cast<int>(maxGravity);
			}
		}
	}
};