#include "../../../include/components/collider/collider.hpp"
#include "../../../include/math/math.hpp"

sf::Vector2f Collider::support( sf::Vector2f direction ) const {
    const float epsilon = 0.000001f;

    sf::Vector2f center = {
        transform.position.x + size.x / 2.f,
        transform.position.y + size.y / 2.f
    };

    float halfWidth = transform.size.x / 2.f;
    float halfHeight = transform.size.y / 2.f;
    float radiusX = halfWidth * shape.x;
    float radiusY = halfHeight * shape.y;
    float rectangleHalfWidth = halfWidth - radiusX;
    float rectangleHalfHeight = halfHeight - radiusY;

    sf::Vector2f point = center;

    if ( direction.x > 0.f ) point.x += rectangleHalfWidth;
    else point.x -= rectangleHalfWidth;

    if ( direction.y > 0.f ) point.y += rectangleHalfHeight;
    else point.y -= rectangleHalfHeight;

    float denominator = std::sqrt(
        radiusX * radiusX * direction.x * direction.x +
        radiusY * radiusY * direction.y * direction.y
    );

    if ( denominator > epsilon ) {
        point.x += radiusX * radiusX * direction.x / denominator;
        point.y += radiusY * radiusY * direction.y / denominator;
    }

    return point;
}

sf::Vector2f Collider::supportMinkowski( const Collider& colliderA, const Collider& colliderB, sf::Vector2f direction ) {
    
    sf::Vector2f pointA = colliderA.support( direction );
    sf::Vector2f pointB = colliderB.support( Math::opposite( direction ) );

    return pointA - pointB;
}


bool Collider::nextSimplex( std::vector<sf::Vector2f>& simplex, sf::Vector2f& direction ) {

    if ( simplex.size() == 2 )
    {
        sf::Vector2f A = simplex[1];
        sf::Vector2f B = simplex[0];

        sf::Vector2f AO = -A;
        sf::Vector2f AB = B - A;

        if ( Math::dotProduct( AB, AO ) > 0.f ) {
            direction = { -AB.y, AB.x };

            if ( Math::dotProduct( direction, AO ) < 0.f) direction = -direction;
        }
        else {
            simplex = { A };
            direction = AO;
        }

        return false;
    }

    if ( simplex.size() == 3 )
    {
        sf::Vector2f A = simplex[2];
        sf::Vector2f B = simplex[1];
        sf::Vector2f C = simplex[0];

        sf::Vector2f AO = -A;
        sf::Vector2f AB = B - A;
        sf::Vector2f AC = C - A;

        sf::Vector2f ABPerpendicular = { -AB.y, AB.x };

        if ( Math::dotProduct( ABPerpendicular, C - A ) > 0.f ) ABPerpendicular = -ABPerpendicular;

        if ( Math::dotProduct( ABPerpendicular, AO ) > 0.f ) {
            simplex = { B, A };
            direction = ABPerpendicular;

            return false;
        }

        sf::Vector2f ACPerpendicular = { AC.y, -AC.x };

        if ( Math::dotProduct( ACPerpendicular, B - A ) > 0.f ) ACPerpendicular = -ACPerpendicular;

        if ( Math::dotProduct( ACPerpendicular, AO ) > 0.f ) {
            simplex = { C, A };
            direction = ACPerpendicular;

            return false;
        }

        return true;
    }

    return false;
}



bool Collider::checkCollision( const Collider& other ) const {

    shape = { shape.x % 1 if shape.x != 1 else 1, shape.y % 1 if shape.y != 1 else 1 }
    other.shape = { shape.x % 1 if shape.x != 1 else 1, shape.y % 1 if shape.y != 1 else 1 }

    sf::Vector2f centerA = {
        transform.position.x + transform.size.x / 2.f,
        transform.position.y + transform.size.y / 2.f
    };

    sf::Vector2f centerB = {
        other.transform.position.x + other.transform.size.x / 2.f,
        other.transform.position.y + other.transform.size.y / 2.f
    };

    sf::Vector2f direction = centerB - centerA;

    if ( direction.x == 0.f && direction.y == 0.f ) direction = { 1.f, 0.f };

    std::vector<sf::Vector2f> simplex;
    sf::Vector2f point = supportMinkowski( *this, other, direction );
    simplex.push_back( point );

    if ( Math::dotProduct( point, direction ) < 0.f ) return false;
    direction = Math::opposite( point );

    const int maxIterations = 16;

    for ( int i = 0; i < maxIterations; ++i ) {

        point = supportMinkowski( *this, other, direction );

        if ( Math::dotProduct( point, direction ) < 0.f ) return false;
        simplex.push_back( point );

        if ( nextSimplex( simplex, direction ) ) return true;
    }

    return false;
}