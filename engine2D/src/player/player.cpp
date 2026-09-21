#include "../../include/player/player.hpp"

Player::Player(Transform& t, PhysicsBody& b, Sprite& s)
    : transform(t), body(b), sprite(s)
{}

void Player::update(const Input& input)
{
    // Déplacement horizontal
    Vector2D movement = input.getMovement();
    body.addForce({ movement.x * input.getSpeed(), 0.f });

    // Saut
    if (input.isJumpPressed()) {
        body.addForce({0.f, 800.f});
    }

    // Accroupi
    if (input.isCrouchPressed()) {
        transform.setScale({0.2f, 0.1f});
    } else {
        transform.setScale({0.2f, 0.2f});
    }
}
