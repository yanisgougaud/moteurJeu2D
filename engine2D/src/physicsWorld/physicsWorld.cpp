#include "../../include/physicsWorld/physicsWorld.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

void PhysicsWorld::removeBody( PhysicsBody& body ) {
    bodies.erase(
        std::remove( bodies.begin(), bodies.end(), &body ),
        bodies.end()
    );
}

float PhysicsWorld::calculateAerodynamicArea( const PhysicsBody& body, const Vector2D& direction ) const {
    
    float projectedWidth = body.getCollider().getProjectedWidth( direction );
    return projectedWidth * body.getAerodynamicDepth();
}

Vector2D PhysicsWorld::calculateDragForce( const PhysicsBody& body, const Vector2D& velocity ) const {
    
    Vector2D relativeAirVelocity = wind - velocity;

    float speedSquared = relativeAirVelocity.lengthSquared();

    if ( speedSquared <= Math::EPSILON ) return { 0.0f, 0.0f };

    float speed = std::sqrt( speedSquared );
    Vector2D direction = relativeAirVelocity / speed;
    float aerodynamicArea = calculateAerodynamicArea( body, direction );

    float dragForceMagnitude =
        0.5f
        * airDensity
        * body.getDragCoefficient()
        * aerodynamicArea
        * speedSquared;

    return direction * dragForceMagnitude;
}

Vector2D PhysicsWorld::solveImplicitVelocity( const PhysicsBody& body, const Vector2D& otherForces ) const {
    
    const Vector2D currentVelocity = body.getVelocity();

    Vector2D newVelocity = currentVelocity;

    constexpr int maxIterations = 32;
    constexpr float relaxation = 0.25f;
    constexpr float convergenceEpsilon = 0.000001f;

    int iterations = 0;

    for ( int i = 0; i < maxIterations; ++i ) {

        ++iterations;
        Vector2D acceleration = otherForces + calculateDragForce( body, newVelocity ) / body.getMass();
        Vector2D nextVelocity = newVelocity * ( 1.0f - relaxation ) + ( currentVelocity + acceleration * fixedDeltaTime ) * relaxation;

        if ( !std::isfinite( nextVelocity.x ) || !std::isfinite( nextVelocity.y ) ) return currentVelocity;

        if ( std::abs( nextVelocity.x < body.getMaterial().getFriction() ) ) nextVelocity.x = 0.0f
        if ( std::abs( nextVelocity.y < body.getMaterial().getFriction() ) ) nextVelocity.y = 0.0f

        Vector2D difference = nextVelocity - newVelocity;
        newVelocity = nextVelocity;

        if ( difference.lengthSquared() <= convergenceEpsilon ) break;
    }

    return newVelocity;
}

void PhysicsWorld::step() {

    for ( PhysicsBody* body : bodies ) {

        if (body == nullptr || body->getIsStatic() ) continue;
        
        body->addForce( gravity * entityGravityScale * body->getMass() );

        Vector2D newVelocity = solveImplicitVelocity( *body, body->getForce() );
        Vector2D acceleration = ( newVelocity - body->getVelocity() ) / fixedDeltaTime;

        body->setAcceleration( acceleration );
        body->setVelocity( newVelocity );
        body->getTransform().setPosition( body->getTransform().getPosition() + newVelocity * fixedDeltaTime );

        std::cout << ( body->getVelocity() * fixedDeltaTime ).x << "   " << ( body->getVelocity() * fixedDeltaTime ).y << "\n";

        body->clearForces();
    }

}
