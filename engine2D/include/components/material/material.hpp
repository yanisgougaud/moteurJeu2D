#pragma once

#include <stdexcept>

class Material {

    float friction;
    float restitution;

public:

    Material(
        float friction = 0.5f,
        float restitution = 0.0f
    )
    {
        setFriction( friction );
        setRestitution( restitution );
    }

    void setFriction( const float newFriction ) {

        if ( newFriction < 0.0f || newFriction > 1.0f ) throw std::runtime_error( "La friction doit etre comprise entre 0 et 1." );
        friction = newFriction;
    }

    float getFriction() const {
        return friction;
    }

    void setRestitution( const float newRestitution ) {

        if ( newRestitution < 0.0f || newRestitution > 1.0f ) throw std::runtime_error( "La restitution doit etre comprise entre 0 et 1." );
        restitution = newRestitution;
    }

    float getRestitution() const {
        return restitution;
    }
};
