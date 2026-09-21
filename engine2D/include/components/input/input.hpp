#pragma once

#include "../../math/vector2D.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <stdexcept>

enum class ControlMode {
    TopDown,
    SideScroll
};

struct MovementKeys {
    sf::Keyboard::Key left  = sf::Keyboard::Key::Q;
    sf::Keyboard::Key right = sf::Keyboard::Key::D;
    sf::Keyboard::Key up    = sf::Keyboard::Key::Z;   // top-down: haut / side-scroll: saut
    sf::Keyboard::Key down  = sf::Keyboard::Key::S;   // top-down: bas / side-scroll: accroupi
};

class Input {

    MovementKeys keys;
    float speed;
    ControlMode mode = ControlMode::TopDown;

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

        // Accélération avec SHIFT
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
            finalSpeed *= 1.8f;

        return finalSpeed;
    }

    Vector2D getMovement() const {
        Vector2D movement{0.f, 0.f};

        if (mode == ControlMode::TopDown)
        {
            if (sf::Keyboard::isKeyPressed(keys.left))  movement.x -= 1.f;
            if (sf::Keyboard::isKeyPressed(keys.right)) movement.x += 1.f;
            if (sf::Keyboard::isKeyPressed(keys.up))    movement.y -= 1.f;
            if (sf::Keyboard::isKeyPressed(keys.down))  movement.y += 1.f;
        }
        else if (mode == ControlMode::SideScroll)
        {
            // Déplacement horizontal
            if (sf::Keyboard::isKeyPressed(keys.left))  movement.x -= 1.f;
            if (sf::Keyboard::isKeyPressed(keys.right)) movement.x += 1.f;

            // Saut → tu devras appeler ton moteur physique ici
            if (sf::Keyboard::isKeyPressed(keys.up)) {
                // Exemple : applyJumpForce();
            }

            // Accroupi / glissade → tu devras modifier la hitbox ou friction
            if (sf::Keyboard::isKeyPressed(keys.down)) {
                // Exemple : setCrouch(true);
            }
        }

        return movement.normalized();
    }
    bool isJumpPressed() const {
    if (mode == ControlMode::SideScroll)
        return sf::Keyboard::isKeyPressed(keys.up);   // Z = saut
    return false;
}

    bool isCrouchPressed() const {
    if (mode == ControlMode::SideScroll)
        return sf::Keyboard::isKeyPressed(keys.down); // S = accroupi
    return false;
}

};
