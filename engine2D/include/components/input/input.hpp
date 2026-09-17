#pragma once

#include "../../math/vector2D.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <stdexcept>

struct MovementKeys {
    sf::Keyboard::Key left  = sf::Keyboard::Key::Q;
    sf::Keyboard::Key right = sf::Keyboard::Key::D;
    sf::Keyboard::Key up    = sf::Keyboard::Key::Z;
    sf::Keyboard::Key down  = sf::Keyboard::Key::S;
};

class Input {

    MovementKeys keys;
    float speed;

public:

    Input(
        MovementKeys movementKeys = {},
        float movementSpeed = 250.0f
    )
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

    void setSpeed(float movementSpeed) {
        if (movementSpeed <= 0.0f)
            throw std::runtime_error("La vitesse doit être strictement positive.");

        speed = movementSpeed;
    }

    float getSpeed() const {
        return speed;
    }

    Vector2D getMovement() const {
        Vector2D movement;

        if (sf::Keyboard::isKeyPressed(keys.left))  movement.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(keys.right)) movement.x += 1.0f;
        if (sf::Keyboard::isKeyPressed(keys.up))    movement.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(keys.down))  movement.y += 1.0f;

        return movement.normalized();
    }
};
