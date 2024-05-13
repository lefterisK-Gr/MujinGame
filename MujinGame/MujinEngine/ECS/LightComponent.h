#pragma once

#include "ECS/ECS.h"
#include "Components.h"

class LightComponent : public Component {
private:
    float _zIndex = 1.0f;
public:
    Color color;
    float radius = 0.0f;

    SDL_Rect destRect;
    TransformComponent* transform = nullptr;

    LightComponent(float zIndex)
    {
        _zIndex = zIndex;
    }

    ~LightComponent() {

    }

    void init() override {
        transform = &entity->GetComponent<TransformComponent>();

        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void update(float deltaTime) override {
        float parallaxFactor = 1.0f / _zIndex;
        destRect.x = static_cast<int>(transform->position.x); //make player move with the camera, being stable in centre, except on edges
        destRect.y = static_cast<int>(transform->position.y);
    }

    void draw(SpriteBatch&  batch, MujinEngine::Window& window) override
    {
        float tempScreenScale = window.getScale();
        glm::vec4 pos((float)destRect.x * tempScreenScale, (float)destRect.y * tempScreenScale,
            (float)destRect.w * tempScreenScale, (float)destRect.h * tempScreenScale);
        batch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color);
	}
};