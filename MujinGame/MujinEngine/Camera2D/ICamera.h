#pragma once

#include <glm/glm.hpp>
#include <SDL/SDL.h>

class ICamera {
public:
    virtual ~ICamera() = default;

    // Initializes the camera2D.worldLocation with the screen's width and height
    virtual void init(int screenWidth, int screenHeight) = 0;

    // Updates the camera2D.worldLocation's matrix if there have been any changes
    virtual void update() = 0;

    // Converts screen coordinates to world coordinates
    virtual glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const = 0;

    // Returns the dimensions of the camera2D.worldLocation's view
    virtual glm::ivec2 getCameraDimensions() const = 0;

    // Returns the SDL_Rect representing the camera2D.worldLocation's viewport
    virtual SDL_Rect getCameraRect() const = 0;

    // Additional methods to expose camera2D.worldLocation properties as needed
    virtual glm::vec2 getPosition() const = 0;
    virtual void setPosition(const glm::vec2& position) = 0;
    virtual float getScale() const = 0;
    virtual glm::mat4 getCameraMatrix() const = 0;
    virtual void setScale(float scale) = 0;
};