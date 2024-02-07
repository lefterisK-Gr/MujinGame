#pragma once

#include "../Components.h"

class HPBar : public Component
{
public:
	float _maxHealthPoints = 100.0f;
	float _healthPoints = 100.0f;

	TransformComponent* transform = nullptr;
	Rectangle_w_Color* rectangle = nullptr;

	HPBar()
	{

	}

	~HPBar() {

	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		if (!entity->hasComponent<Rectangle_w_Color>()) {
			entity->addComponent<Rectangle_w_Color>();
		}
		rectangle = &entity->getComponent<Rectangle_w_Color>();
	}

	void update(float deltaTime) override {
		rectangle->destRect.w = transform->width * transform->scale * (_healthPoints / _maxHealthPoints);
	}

	void draw(SpriteBatch& batch) override {
	}
};