#pragma once

#include "../../math/Vector2D.hpp"

#include <vector>

class Collider {

    std::vector<Vector2D> vertices;
    Transform& transform;
    Vector2D size;
    unsigned int lastTransformVersion = 0;
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

public:

    Collider(
        Transform& transform,
        std::vector<Vector2D> vertices = {}
    )
        : transform( transform ),
          vertices( vertices ),
          size(getSize())
    {
    }

    void setVertices( const std::vector<Vector2D>& newVertices ) {
        for ( Vector2D newVertice : newVertices )
            if ( newVertice.x < 0.0f || newVertice.y < 0.0f ) throw std::runtime_error( "Les sommets de la figure doivent être positifs." );
        vertices = newVertices;
        ++lastVersion;
    }

    void addVertice( const Vector2D& newVertice ) {
        if ( newVertice.x < 0.0f || newVertice.y < 0.0f ) throw std::runtime_error( "Les sommets de la figure doivent être positifs." );
        vertices.insert( vertices.end(), newVertices );
        ++lastVersion;
    }

    void addVertices( const std::vector<Vector2D>& newVertices ) {
        for ( Vector2D newVertice : newVertices )
            if ( newVertice.x < 0.0f || newVertice.y < 0.0f ) throw std::runtime_error( "Les sommets de la figure doivent être positifs." );
        vertices.insert( vertices.end() , newVertices.begin(), newVertices.end() );
        ++lastVersion;
    }

    const std::vector<Vector2D>& getVertices() const {
        return vertices;
    }

    const Vector2D& getSize() const;

    bool checkCollision( const Collider& otherCollider ) const;

    const unsigned int getLastVersion() const {
        return lastVersion
    }
    
};
