#pragma once

#include "ECS/ECS.h"
#include "Components.h"

class LightTextureComponent : public Component {
private:
    float _zIndex = 1.0f;
public:
    float radius = 100.0f;

    SDL_Rect destRect;
    TransformComponent* transform = nullptr;
    SpriteComponent* sprite;

    LightTextureComponent(float zIndex)
    {
        _zIndex = zIndex;
    }

    ~LightTextureComponent() {

    }

    void init() override {
        if (!entity->hasComponent<SpriteComponent>()) //todo: problem: having transform on top left grid, not every collider its own
        {
            entity->addComponent<SpriteComponent>();
        }
        transform = &entity->GetComponent<TransformComponent>();
        sprite = &entity->GetComponent<SpriteComponent>();

        destRect.w = radius;
        destRect.h = radius;
    }

    void update(float deltaTime) override {
        float parallaxFactor = 1.0f / _zIndex;
        destRect.x = static_cast<int>(transform->position.x) - (Game::camera.x * parallaxFactor); //make player move with the camera, being stable in centre, except on edges
        destRect.y = static_cast<int>(transform->position.y) - (Game::camera.y * parallaxFactor);
    }

    void draw(SpriteBatch& batch) override
    {
        float tempScreenScale = Game::_window->getScale();
        glm::vec4 pos((float)destRect.x * tempScreenScale, (float)destRect.y * tempScreenScale,
            (float)destRect.w * tempScreenScale, (float)destRect.h * tempScreenScale);
        batch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, sprite->color);
    }
};