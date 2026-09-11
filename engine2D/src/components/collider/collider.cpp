#include "../../../include/components/collider/collider.hpp"
#include "../../../include/components/transform/transform.hpp"
#include "../../../include/math/math.hpp"

#include <cmath>
#include <limits>
#include <algorithm>


const Vector2D& Collider::getSize()
{
    if ( vertices.empty() )  return { 0.0f, 0.0f };

    if ( transform.getLastVersion() != lastTransformVersion ) {

        const float cosAngle = std::cos( transform.getRadiantAngle() );
        const float sinAngle = std::sin( transform.getRadiantAngle() );

        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();

        float maxX = std::numeric_limits<float>::lowest();
        float maxY = std::numeric_limits<float>::lowest();

        for ( const Vector2D& vertex : vertices ) {
            Vector2D scaledVertex = {
                vertex.x * transform.scale.x,
                vertex.y * transform.scale.y
            };

            Vector2D rotatedVertex = {
                scaledVertex.x * cosAngle - scaledVertex.y * sinAngle,
                scaledVertex.x * sinAngle + scaledVertex.y * cosAngle
            };

            minX = std::min( minX, rotatedVertex.x );
            minY = std::min( minY, rotatedVertex.y );

            maxX = std::max( maxX, rotatedVertex.x );
            maxY = std::max( maxY, rotatedVertex.y );
        }

        size = {
            maxX - minX,
            maxY - minY
        };
        lastTransformVersion = transform.getLastVersion();
    }

    return size;
}

Vector2D Collider::support( const Vector2D direction ) const {

    if ( vertices.empty() )  return transform.position;

    const float cosAngle = std::cos( transform.radiantAngle );
    const float sinAngle = std::sin( transform.radiantAngle );

    Vector2D farthestPoint;
    float maxDotProduct = std::numeric_limits<float>::lowest();

    for ( const Vector2D& vertex : vertices ) {

        Vector2D scaledVertex = {
            vertex.x * transform.scale.x,
            vertex.y * transform.scale.y
        };

        Vector2D rotatedVertex = {
            scaledVertex.x * cosAngle - scaledVertex.y * sinAngle,
            scaledVertex.x * sinAngle + scaledVertex.y * cosAngle
        };

        Vector2D placedVertex = {
            transform.position.x + rotatedVertex.x,
            transform.position.y + rotatedVertex.y
        };

        float dotProduct = placedVertex.dotProduct( direction );

        if ( dotProduct > maxDotProduct ) {
            maxDotProduct = dotProduct;
            farthestPoint = placedVertex;
        }
    }

    return farthestPoint;
}

Vector2D Collider::supportMinkowski(
    const Collider& colliderA,
    const Collider& colliderB,
    const Vector2D direction
)
{
    Vector2D pointA = colliderA.support( direction );
    Vector2D pointB = colliderB.support( -direction );

    return pointA - pointB;
}

bool Collider::nextSimplex(
    std::vector<Vector2D>& simplex,
    Vector2D& direction
)
{

    if ( simplex.size() == 2 ) {

        Vector2D A = simplex[1];
        Vector2D B = simplex[0];
        Vector2D AO = -A;
        Vector2D AB = B - A;

        if ( std::abs( AB.cross( AO ) ) < Math::EPSILON )
            if ( AO.dotProduct( AB ) >= - Math::EPSILON && AO.lengthSquared() <= AB.lengthSquared() ) return true;

        if ( AB.dotProduct( AO ) > Math::EPSILON ) {
            direction = {
                -AB.y,
                AB.x
            };

            if ( direction.dotProduct( AO ) < - Math::EPSILON ) direction = -direction; 
        }
        else
        {
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

    Vector2D centerA = transform.position + size.x / 2;
    Vector2D centerB = other.transform.position + other.size.x / 2;
    Vector2D direction = centerB - centerA;

    if ( direction.x == 0.0f && direction.y == 0.0f ) direction = { 1.0f, 0.0f };

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

    for (int i = 0; i < maxIterations; ++i) {

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
