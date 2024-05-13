#pragma once

#include "Components.h"

class Rectangle_w_Color : public Component
{
public:
	Color color;
	float rotation = 0.0f;

	SDL_Rect destRect;
	TransformComponent* transform = nullptr;

	Rectangle_w_Color()
	{

	}

	~Rectangle_w_Color() {

	}

	void init() override {
		transform = &entity->GetComponent<TransformComponent>();

		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void update(float deltaTime) override {
		std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

		destRect.x = static_cast<int>(transform->position.x) - main_camera2D->worldLocation.x; //make player move with the camera, being stable in centre, except on edges
		destRect.y = static_cast<int>(transform->position.y) - main_camera2D->worldLocation.y;
	}

	void draw(SpriteBatch& batch) override {
		float tempScreenScale = Game::_window->getScale();
		glm::vec4 pos((float)destRect.x * tempScreenScale, (float)destRect.y * tempScreenScale, (float)destRect.w * tempScreenScale, (float)destRect.h * tempScreenScale);
		batch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, rotation);
	}
};