#pragma once

#include "Components.h"
#include <functional>  // For std::function

class ButtonComponent : public Component {
public:
    enum class ButtonState {
        NORMAL,
        HOVERED,
        PRESSED
    };

private:
    ButtonState _state;
    std::function<void()> _onClick; // Callback function for click action

public:
    ButtonComponent(std::function<void()> onClick)
        : _state(ButtonState::NORMAL), _onClick(onClick) {}

    ButtonComponent(std::function<void()> onClick, std::string button_label, glm::ivec2 b_dimensions, Color b_background)
        : _state(ButtonState::NORMAL), _onClick(onClick) {
           // create UI label with the string given
           // set the button background color
    }

    void setOnClick(std::function<void()> newOnClick) {
        _onClick = newOnClick;
    }

    void setState(ButtonState state) {
        _state = state;
    }

    void update(float deltaTime) override {
        // Update the button state based on user input
        // Change the texture or appearance based on the state
        // ...

        if (_state == ButtonState::PRESSED && _onClick) {
            _onClick(); // Call the click action callback
        }
        setState(ButtonState::NORMAL); // Reset the state
    }

    // Other methods for setting textures, handling mouse events, etc.
};