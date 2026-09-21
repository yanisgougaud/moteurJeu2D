#include "../../../include/components/input/input.hpp"

#include <SFML/Graphics.hpp>



Vector2D Input::getMovement() const {
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
        if (sf::Keyboard::isKeyPressed(keys.left))  movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(keys.right)) movement.x += 1.f;
    }

    return movement.normalized();
}

