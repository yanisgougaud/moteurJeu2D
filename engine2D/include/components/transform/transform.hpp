#pragma once

#include "../../math/Vector2D.hpp"

struct Transform {

    Vector2D position;
    Vector2D scale;
    float rotation;
    float radiantAngle;

    Transform(
        Vector2D position = { 0.0f, 0.0f },
        Vector2D scale = { 1.0f, 1.0f },
        float rotation = 0.0f
    )
        : position( position ),
          scale( scale ),
          rotation( rotation ),
          radiantAngle( radiantAngle = rotation * M_PI / 180.0f )
    {
    }
};
