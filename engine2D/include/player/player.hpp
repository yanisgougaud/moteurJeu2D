#pragma once

#include "../components/input/input.hpp"
#include "../components/transform/transform.hpp"
#include "../components/physicsBody/physicsBody.hpp"
#include "../components/sprite/sprite.hpp"

class Player {

public:
    Transform& transform;
    PhysicsBody& body;
    Sprite& sprite;

    Player(Transform& t, PhysicsBody& b, Sprite& s);

    void update(const Input& input);
};
