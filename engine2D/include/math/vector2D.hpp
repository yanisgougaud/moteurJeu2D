#pragma once

#include <cmath>

struct Vector2D
{
    float x;
    float y;

    Vector2D( float x = 0.0f, float y = 0.0f )
        : x( x ),
          y( y )
    {
    }

    Vector2D operator+( const Vector2D& other ) const {
        return {
            x + other.x,
            y + other.y
        };
    }

    Vector2D operator-( const Vector2D& other ) const {
        return {
            x - other.x,
            y - other.y
        };
    }

    Vector2D operator-() const {
        return {
            -x,
            -y
        };
    }

    Vector2D operator*( float scalar ) const {
        return {
            x * scalar,
            y * scalar
        };
    }

    Vector2D operator/( float scalar ) const {
        return {
            x / scalar,
            y / scalar
        };
    }

    Vector2D& operator+=( const Vector2D& other ) {
        x += other.x;
        y += other.y;

        return *this;
    }

    Vector2D& operator-=( const Vector2D& other ) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Vector2D& operator*=( float scalar ) {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    float length() const {
        return std::sqrt( x * x + y * y );
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    Vector2D normalized() const {
        float length = this->length();

        if (length == 0.0f) return { 0.0f, 0.0f };

        return {
            x / length,
            y / length
        };
    }

    float dotProduct( const Vector2D& other ) const {
        return x * other.x + y * other.y;
    }

    float cross(const Vector2D& other) const {
        return x * other.y - y * other.x;
    }

    Vector2D perpendicular() {
        return {
            AB.y,
            AB.x
        }
    }
};