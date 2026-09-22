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
        airDensity
        * body.getDragCoefficient()
        * aerodynamicArea
        * speedSquared;

    return direction * dragForceMagnitude;
}

Vector2D PhysicsWorld::solveImplicitVelocity( const PhysicsBody& body, const Vector2D& otherForces ) const {
    
    const Vector2D currentVelocity = body.getVelocity();

    Vector2D newVelocity = currentVelocity;

    constexpr int maxIterations = 32;

    for ( int i = 0; i < maxIterations; ++i ) {

        Vector2D acceleration = otherForces + calculateDragForce( body, newVelocity ) / body.getMass();
        
        Vector2D targetVelocity = currentVelocity + acceleration * fixedDeltaTime;

        float currentSpeedSquared = newVelocity.lengthSquared();
        float targetSpeedSquared = targetVelocity.lengthSquared();

        float relaxation = targetSpeedSquared > currentSpeedSquared ? body.getAccelerationRelaxation() : body.getDecelerationRelaxation();
        
        Vector2D nextVelocity = newVelocity * ( 1.0f - relaxation ) + ( currentVelocity + acceleration * fixedDeltaTime ) * relaxation;

        if ( !std::isfinite( nextVelocity.x ) || !std::isfinite( nextVelocity.y ) ) return currentVelocity;

        Vector2D difference = nextVelocity - newVelocity;
        newVelocity = nextVelocity;

        if ( difference.lengthSquared() <= Math::EPSILON ) break;
    }

    return newVelocity;
}

void PhysicsWorld::step() {

    for ( PhysicsBody* body : bodies ) {

        if ( body == nullptr || body->getIsStatic() ) continue;
        
        body->addForce( gravity * entityGravityScale * body->getMass() );
        body->addForce( { body->getInput().getMovement().x * body->getInput().getSpeed(), body->getInput().isJumpPressed() ? 100.0f : 0.0f } );

        Vector2D newVelocity = solveImplicitVelocity( *body, body->getForce() );
        Vector2D acceleration = ( newVelocity - body->getVelocity() ) / fixedDeltaTime;

        if ( std::abs( acceleration.x ) < body->getMaterial().getFriction() * 10 && std::abs( newVelocity.x ) < body->getMaterial().getFriction() * 50 ) newVelocity.x = 0.0f;
        if ( std::abs( acceleration.y ) < body->getMaterial().getFriction() * 10 && std::abs( newVelocity.y ) < body->getMaterial().getFriction() * 50 ) newVelocity.y = 0.0f;

        std::cout << "Acceleration : " << acceleration.x << "   " << acceleration.y << "\n";
        std::cout << "Velocity : " << newVelocity.x << "   " << newVelocity.y << "\n";

        body->setAcceleration( acceleration );
        body->setVelocity( newVelocity );
        body->getTransform().setPosition( body->getTransform().getPosition() + newVelocity * fixedDeltaTime );

        std::cout << ( body->getVelocity() * fixedDeltaTime ).x << "   " << ( body->getVelocity() * fixedDeltaTime ).y << "\n";

        body->clearForces();
    }

}
