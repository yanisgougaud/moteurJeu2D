#pragma once

#include <SFML/System/Vector2.hpp>

struct Transform {

    sf::Vector2<float> position;
    sf::Vector2<float> scale;
    float rotation;

    Transform(
        sf::Vector2<float> position = { 0.0f, 0.0f },
        sf::Vector2<float> scale = { 1.0f, 1.0f },
        float rotation = 0.0f
    )
        : position( position ),
          scale( scale ),
          rotation( rotation )
    {
    }
};
