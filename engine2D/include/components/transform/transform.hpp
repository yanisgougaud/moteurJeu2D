#pragma once

#include "../../math/Vector2D.hpp"
#include "../../math/math.hpp"

#include <stdexcept>
#include <cmath>

class Transform {

    Vector2D position;
    Vector2D scale;
    Vector2D origin;
    Vector2D centerOfGravity;
    float rotation;
    float radianAngle;
    unsigned int lastVersion = 0;

public:

    Transform(
        Vector2D position = { 0.0f, 0.0f },
        Vector2D scale = { 1.0f, 1.0f },
        Vector2D origin = { 0.0f, 0.0f },
        Vector2D centerOfGravity = { 0.0f, 0.0f },
        float rotation_ = 0.0f,
        float radianAngle_ = 0.0f
    )
        : position( position ),
          scale( scale ),
          origin( origin ),
          centerOfGravity( centerOfGravity ),
          rotation( 0.0f ),
          radianAngle( 0.0f )
    {
        if ( rotation_ == 0.0f && radianAngle_ != 0.0f ) setRadianAngle( radianAngle_ );
        else setRotation( rotation_ );
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

    float getX() const {
        return position.x;
    }

    float getY() const {
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

    float getScaleX() const {
        return scale.x;
    }

    float getScaleY() const {
        return scale.y;
    }

    void setOrigin( const Vector2D& newOrigin ) {
        origin = newOrigin;
        ++lastVersion;
    }

    void setOriginX( const float newOriginX ) {
        origin.x = newOriginX;
        ++lastVersion;
    }

    void setOriginY( const float newOriginY ) {
        origin.y = newOriginY;
        ++lastVersion;
    }

    const Vector2D& getOrigin() const {
        return origin;
    }

    float getOriginX() const {
        return origin.x;
    }

    float getOriginY() const {
        return origin.y;
    }

    void setCenterOfGravity( const Vector2D& newCenterOfGravity ) {
        centerOfGravity = newCenterOfGravity;
        ++lastVersion;
    }

    void setCenterOfGravityX( const float newCenterOfGravityX ) {
        centerOfGravity.x = newCenterOfGravityX;
        ++lastVersion;
    }

    void setCenterOfGravityY( const float newCenterOfGravityY ) {
        centerOfGravity.y = newCenterOfGravityY;
        ++lastVersion;
    }

    const Vector2D& getCenterOfGravity() const {
        return centerOfGravity;
    }

    float getCenterOfGravityX() const {
        return centerOfGravity.x;
    }

    float getCenterOfGravityY() const {
        return centerOfGravity.y;
    }

    void setRotation( const float newRotation ) {
        rotation = std::fmod( newRotation, 360.0f );
        if ( rotation < 0.0f ) rotation += 360.0f;
        radianAngle = rotation * Math::PI / 180.0f;
        ++lastVersion;
    }

    float getRotation() const {
        return rotation;
    }

    void setRadianAngle( const float newRadianAngle ) {
        radianAngle =  std::fmod( newRadianAngle, 2 * Math::PI );
        if ( radianAngle < 0.0f ) radianAngle += 2.0f * Math::PI;
        rotation = radianAngle * 180.0f / Math::PI;
        ++lastVersion;
    }

    float getRadianAngle() const {
        return radianAngle;
    }

    unsigned int getLastVersion() const {
        return lastVersion;
    }

};
