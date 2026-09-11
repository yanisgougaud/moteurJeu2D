#pragma once

#include "../../math/Vector2D.hpp"

#include <stdexcept>
#include <cmath>

class Transform {

    Vector2D position;
    Vector2D scale;
    float rotation;
    float radiantAngle;
    unsigned int lastVersion = 0;

public:

    Transform(
        Vector2D position = { 0.0f, 0.0f },
        Vector2D scale = { 1.0f, 1.0f },
        float rotation = 0.0f,
        float radiantAngle = 0.0f
    )
        : position( position ),
          scale( scale )
    {
        if ( rotation == 0.0f && radiantAngle != 0.0f ) setRadiantAngle();
        else setRotation();
    }

    void setPosition( const Vector2D& newPosition ) {
        position = newPosition;
        ++lastVersion;
    }

    void setX( const float newX ) {
        position.x = newX;
        ++lastVersion;
    }

    void setY( const float newY ) {
        position.y = newY;
        ++lastVersion;
    }

    const Vector2D& getPosition() const {
        return position;
    }

    const float& getX() const {
        return position.x;
    }

    const float& getY() const {
        return position.y;
    }

    void setScale( const Vector2D& newScale ) {
        if ( newScale.x <= 0 || newScale.y <= 0 ) throw std::runtime_error( "Le scale doit être strictement positif." );
        scale = newScale;
        ++lastVersion;
    }

    void setScaleX( const float newScaleX ) {
        if ( newScaleX <= 0 ) throw std::runtime_error( "Le scale x doit être strictement positif." );
        scale.x = newScaleX;
        ++lastVersion;
    }

    void setScaleY( const float newScaleY ) {
        if ( newScaleY <= 0 ) throw std::runtime_error( "Le scale y doit être strictement positif." );
        scale.y = newScaleY;
        ++lastVersion;
    }

    const Vector2D& getScale() const {
        return scale;
    }

    const float& getScaleX() const {
        return scale.x;
    }

    const float& getScaleY() const {
        return scale.y;
    }

    const setRotation( const float newRotation ) {
        rotation = std::abs( std::fmod( newRotation, 360.0f ) );
        radiantAngle = rotation * M_PI / 180.0f;
        ++lastVersion;
    }

    const float& getRotation() const {
        return rotation;
    }

    const setRadiantAngle( const float newRadiantAngle ) {
        radiantAngle = std::abs( std::fmod( newRadiantAngle, 2 * M_PI ) );
        radiantAngle = rotation * 180.0f / M_PI;
        ++lastVersion;
    }

    const float& getRadiantAngle() const {
        return radiantAngle;
    }

    const unsigned int getLastVersion() const {
        return lastVersion
    }

};
