#pragma once

#include "../../math/vector2D.hpp"

#include <stdexcept>
#include <SFML/Window/Keyboard.hpp>

enum class ControlMode {
    TopDown,
    SideScroll
};

struct MovementKeys {
    sf::Keyboard::Key left  = sf::Keyboard::Key::Q;
    sf::Keyboard::Key right = sf::Keyboard::Key::D;
    sf::Keyboard::Key up    = sf::Keyboard::Key::Z;
    sf::Keyboard::Key down  = sf::Keyboard::Key::S;
};

class Input {

    MovementKeys keys;
    float speed;
    ControlMode mode = ControlMode::TopDown;

public:

    Input(MovementKeys movementKeys, float movementSpeed)
        : keys(movementKeys)
    {
        setSpeed(movementSpeed);
    }

void setKeys(const MovementKeys& movementKeys) {
    keys = movementKeys;
}

const MovementKeys& getKeys() const {
    return keys;
}

void setMode(ControlMode newMode) {
    mode = newMode;
}

ControlMode getMode() const {
    return mode;
}

void setSpeed(float movementSpeed) {
    if (movementSpeed <= 0.0f)
        throw std::runtime_error("La vitesse doit être strictement positive.");
    speed = movementSpeed;
}

float getSpeed() const {
    float finalSpeed = speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
        finalSpeed *= 1.8f;

    return finalSpeed;
}

    Vector2D getMovement() const;


bool isJumpPressed() const {
    return mode == ControlMode::SideScroll &&
           sf::Keyboard::isKeyPressed(keys.up);
}

bool isCrouchPressed() const {
    return mode == ControlMode::SideScroll &&
           sf::Keyboard::isKeyPressed(keys.down);
}

};
