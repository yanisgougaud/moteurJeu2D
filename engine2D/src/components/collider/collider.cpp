#include "../../../include/components/collider/collider.hpp"
#include "../../../include/components/transform/transform.hpp"
#include "../../../include/math/math.hpp"

#include <cmath>
#include <limits>
#include <algorithm>

void Collider::calculateSize() {

    if ( vertices.empty() ) {
        size = { 0.0f, 0.0f };
        return;
    }

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();

    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for ( const Vector2D& vertex : vertices ) {

        minX = std::min( minX, vertex.x );
        minY = std::min( minY, vertex.y );

        maxX = std::max( maxX, vertex.x );
        maxY = std::max( maxY, vertex.y );
    }

    size = {
        maxX - minX,
        maxY - minY
    };
}

Vector2D Collider::support( const Vector2D direction ) const {

    if ( vertices.empty() ) return transform.getPosition();

    Vector2D farthestPoint;
    float maxDotProduct = std::numeric_limits<float>::lowest();

    for ( const Vector2D& vertex : vertices ) {

        Vector2D finalPosition = transformVertex( vertex );

        float dotProduct = finalPosition.dotProduct(direction);

        if (dotProduct > maxDotProduct) {
            maxDotProduct = dotProduct;
            farthestPoint = finalPosition;
        }
    }

    return farthestPoint;
}

Vector2D Collider::transformVertex( const Vector2D& vertex ) const {

    Vector2D vertexPosition = {
        vertex.x - transform.getOriginX(),
        vertex.y - transform.getOriginY()
    };

    vertexPosition.x *= transform.getScaleX();
    vertexPosition.y *= transform.getScaleY();

    Vector2D centerOfGravity = {
        ( transform.getCenterOfGravityX() - transform.getOriginX() ) * transform.getScaleX(),
        ( transform.getCenterOfGravityY() - transform.getOriginY() ) * transform.getScaleY()
    };

    Vector2D relativePosition = vertexPosition - centerOfGravity;
    const float cosAngle = std::cos( transform.getRadianAngle() );
    const float sinAngle = std::sin( transform.getRadianAngle() );

    Vector2D rotatedPosition = {
        relativePosition.x * cosAngle - relativePosition.y * sinAngle,
        relativePosition.x * sinAngle + relativePosition.y * cosAngle
    };

    return rotatedPosition + centerOfGravity + transform.getPosition();
}


Vector2D Collider::supportMinkowski( const Collider& colliderA, const Collider& colliderB, const Vector2D direction ) {
    
    Vector2D pointA = colliderA.support( direction );
    Vector2D pointB = colliderB.support( -direction );

    return pointA - pointB;
}

bool Collider::nextSimplex( std::vector<Vector2D>& simplex, Vector2D& direction ) {

    if ( simplex.size() == 2 ) {

        Vector2D A = simplex[1];
        Vector2D B = simplex[0];
        Vector2D AO = -A;
        Vector2D AB = B - A;

        if ( std::abs( AB.cross( AO ) ) < Math::EPSILON )
            if ( AO.dotProduct( AB ) >= - Math::EPSILON && AO.lengthSquared() <= AB.lengthSquared() ) return true;

        if ( AB.dotProduct( AO ) > Math::EPSILON ) {
            direction = AB.perpendicular();

            if ( direction.dotProduct( AO ) < - Math::EPSILON ) direction = -direction; 
        }
        else {
            simplex = { A };
            direction = AO;
        }

        return false;
    }

    if ( simplex.size() == 3 ) {

        Vector2D A = simplex[2];
        Vector2D B = simplex[1];
        Vector2D C = simplex[0];
        Vector2D AO = -A;
        Vector2D AB = B - A;
        
        Vector2D ABPerpendicular = AB.perpendicular();

        if ( ABPerpendicular.dotProduct( C - A ) > Math::EPSILON) ABPerpendicular = -ABPerpendicular;

        if ( ABPerpendicular.dotProduct( AO ) > Math::EPSILON ) {
            
            simplex = { B, A };
            direction = ABPerpendicular;

            return false;
        }

        Vector2D AC = C - A;
        Vector2D ACPerpendicular = AC.perpendicular();

        if ( ACPerpendicular.dotProduct( B - A ) > Math::EPSILON ) ACPerpendicular = -ACPerpendicular;

        if ( ACPerpendicular.dotProduct( AO ) > Math::EPSILON )  {
            simplex = { C, A };
            direction = ACPerpendicular;

            return false;
        }
        return true;
    }

    return false;
}

bool Collider::checkCollision( const Collider& other ) const {

    if ( vertices.empty() || other.vertices.empty() ) return false;

    Vector2D direction = other.transform.getPosition() - transform.getPosition();
    
    if ( std::abs( direction.x ) < Math::EPSILON && std::abs( direction.y ) < Math::EPSILON ) direction = { 1.0f, 0.0f };

    Vector2D point = supportMinkowski(
        *this,
        other,
        direction
    );

    if ( point.dotProduct( direction ) < - Math::EPSILON ) return false;

    std::vector<Vector2D> simplex;
    simplex.push_back( point );
    direction = -point;

    constexpr int maxIterations = 16;

    for ( int i = 0; i < maxIterations; ++i ) {

        point = supportMinkowski(
            *this,
            other,
            direction
        );

        if ( point.dotProduct( direction ) < - Math::EPSILON ) return false;

        simplex.push_back( point );

        if ( nextSimplex( simplex, direction ) )  return true;
    }

    return false;
}

float Collider::getProjectedWidth( const Vector2D& direction ) const {
    
    if ( vertices.empty() || direction.lengthSquared() <= Math::EPSILON ) return 0.0f;

    Vector2D copyDirection = direction;
    Vector2D perpendicular = copyDirection.perpendicular().normalized();

    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    for ( const Vector2D& vertex : vertices ) {

        Vector2D finalPosition = transformVertex( vertex );

        float projection = finalPosition.dotProduct( perpendicular );

        minProjection = std::min(minProjection, projection);
        maxProjection = std::max(maxProjection, projection);
    }

    return maxProjection - minProjection;
}

