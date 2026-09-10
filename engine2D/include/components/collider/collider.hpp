#pragma once

#include "../../math/Vector2D.hpp"

#include <vector>

struct Collider {

    Transform& transform;
    std::vector<Vector2D> vertices;

    Collider(
        Transform& transform,
        std::vector<Vector2D> vertices = {}
    )
        : transform( transform ),
          vertices( vertices ),
          size(0.0f, 0.0f)
    {
        calculateSize();
    }

    bool checkCollision( const Collider& otherCollider ) const;


private:

    Vector2D size;

    unsigned int lastTransformVersion = 0;

    calculateSize();

    Vector2D support( Vector2D direction ) const;

    static Vector2D supportMinkowski(
        const Collider& a,
        const Collider& b,
        Vector2D direction
    );

    static bool nextSimplex(
        std::vector<Vector2D>& simplex,
        Vector2D& direction
    );
};
