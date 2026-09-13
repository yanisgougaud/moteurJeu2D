#pragma once

#include "../transform/transform.hpp"
#include "../../math/Vector2D.hpp"

#include <vector>

class Collider {

    std::vector<Vector2D> vertices;
    Transform& transform;
    Vector2D size;
    unsigned int lastVersion = 0;

    Vector2D support( Vector2D direction ) const;

    static Vector2D supportMinkowski(
        const Collider& colliderA,
        const Collider& colliderB,
        Vector2D direction
    );

    static bool nextSimplex(
        std::vector<Vector2D>& simplex,
        Vector2D& direction
    );

    void calculateSize();

public:

    Collider(
        Transform& transform,
        std::vector<Vector2D> vertices = {}
    )
        : transform( transform ),
          vertices( vertices )
    {
        calculateSize();
    }

    void setVertices( const std::vector<Vector2D>& newVertices ) {
        vertices = newVertices;
        calculateSize();
        ++lastVersion;
    }

    void addVertice( const Vector2D& newVertice ) {
        vertices.push_back(newVertice);
        calculateSize();
        ++lastVersion;
    }

    void addVertices( const std::vector<Vector2D>& newVertices ) {
        vertices.insert( vertices.end() , newVertices.begin(), newVertices.end() );
        calculateSize();
        ++lastVersion;
    }

    const std::vector<Vector2D>& getVertices() const {
        return vertices;
    }

    const Vector2D& getSize() const {
        return size;
    }

    bool checkCollision( const Collider& otherCollider ) const;

    const unsigned int getLastVersion() const {
        return lastVersion;
    }
    
};
