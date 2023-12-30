#pragma once

#include "./ECS.h"
#include "Components.h"

class LightComponent : public Component {
private:
    float _zIndex = 1.0f;
public:
    Color color;

    SDL_Rect destRect;
    TransformComponent* transform = nullptr;

    LightComponent(float zIndex)
    {
        _zIndex = zIndex;
    }

    ~LightComponent() {
        // Destructor, if needed
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();

        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void update(float deltaTime) override {
        float parallaxFactor = 1.0f / _zIndex;
        destRect.x = static_cast<int>(transform->position.x) - (Game::camera.x * parallaxFactor); //make player move with the camera, being stable in centre, except on edges
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
    }

    void draw() override
    {
        glm::vec4 pos((float)destRect.x, -640 + (float)destRect.y, (float)destRect.w, (float)destRect.h);
		// Draw light, if needed
        Game::_spriteBatch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color);
	}
};