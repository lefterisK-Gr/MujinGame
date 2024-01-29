#pragma once

#include "../Components.h"

class HPBar : public Component
{
public:
	float _maxHealthPoints = 100.0f;
	float _healthPoints = 100.0f;

	Color color;

	SDL_Rect destRect;
	TransformComponent* transform = nullptr;

	HPBar()
	{

	}

	~HPBar() {

	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void update(float deltaTime) override {
		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;

		destRect.w = transform->width * transform->scale * (_healthPoints / _maxHealthPoints);
	}

	void draw(SpriteBatch& batch) override {
		float tempScreenScale = Game::_window->getScale();
		glm::vec4 pos((float)destRect.x* tempScreenScale, -Game::_window->getScreenHeight() + (float)destRect.y * tempScreenScale, (float)destRect.w * tempScreenScale, (float)destRect.h * tempScreenScale);
		batch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color);
	}
};