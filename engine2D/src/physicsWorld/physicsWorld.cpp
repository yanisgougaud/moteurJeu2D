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

    detectCollisions();

    for ( PhysicsBody* body : bodies ) {

        if ( body == nullptr || body->getIsStatic() ) continue;
        
        body->addForce( gravity * entityGravityScale * body->getMass() );
        body->addForce( { body->getInput().getMovement().x * body->getInput().getSpeed(), body->getInput().isJumpPressed() ? 100.0f : 0.0f } );

        Vector2D newVelocity = solveImplicitVelocity( *body, body->getForce() );
        Vector2D acceleration = ( newVelocity - body->getVelocity() ) / fixedDeltaTime;

        if ( std::abs( acceleration.x ) < body->getMaterial().getFriction() * 10 && std::abs( newVelocity.x ) < body->getMaterial().getFriction() * 50 ) newVelocity.x = 0.0f;
        if ( std::abs( acceleration.y ) < body->getMaterial().getFriction() * 10 && std::abs( newVelocity.y ) < body->getMaterial().getFriction() * 50 ) newVelocity.y = 0.0f;

        body->setAcceleration( acceleration );
        body->setVelocity( newVelocity );
        body->getTransform().setPosition( body->getTransform().getPosition() + newVelocity * fixedDeltaTime );

        body->clearForces();
    }

}

void PhysicsWorld::detectCollisions( int maxIterations ) const {

    for (int iteration = 0; iteration < maxIterations; ++iteration) {

        for ( std::size_t i = 0; i < bodies.size(); ++i ) {
            
            PhysicsBody* bodyA = bodies[i];

            if ( bodyA == nullptr ) continue;

            for ( std::size_t j = i + 1; j < bodies.size(); ++j ) {
                
                PhysicsBody* bodyB = bodies[j];

                if ( bodyB == nullptr ) continue;

                GJKResult gjkResult = bodyA->getCollider().runGJK( bodyB->getCollider() );

                if ( !gjkResult.getHasCollision() ) continue;

                EPAResult epaResult = bodyA->getCollider().runEPA( bodyB->getCollider(), gjkResult.getSimplex() );

                if ( !epaResult.getIsValid() ) continue;

                correctPenetration( *bodyA, *bodyB, epaResult.getNormal(), epaResult.getPenetrationDepth() );
                applyCollisionImpulse( *bodyA, *bodyB, epaResult.getNormal() );

            }
        }
    }
}


void PhysicsWorld::correctPenetration( PhysicsBody& bodyA, PhysicsBody& bodyB, const Vector2D& normal, float penetrationDepth ) const {

    constexpr float penetrationSlop = 0.001f;
    constexpr float correctionPercent = 1.0f;

    float correctionDepth = penetrationDepth - Math::EPSILON;

    if ( correctionDepth <= 0.0f ) return;

    correctionDepth *= correctionPercent;

    float inverseMassA = bodyA.getIsStatic() ? 0.0f : 1.0f / bodyA.getMass();
    float inverseMassB = bodyB.getIsStatic() ? 0.0f : 1.0f / bodyB.getMass();
    float inverseMassSum = inverseMassA + inverseMassB;

    if ( inverseMassSum <= Math::EPSILON ) return;

    Vector2D correction = normal * ( correctionDepth / inverseMassSum );
    Vector2D positionA = bodyA.getTransform().getPosition();
    Vector2D positionB = bodyB.getTransform().getPosition();
    positionA -= correction * inverseMassA;
    positionB += correction * inverseMassB;

    bodyA.getTransform().setPosition( positionA );
    bodyB.getTransform().setPosition( positionB );
}

void PhysicsWorld::applyCollisionImpulse( PhysicsBody& bodyA, PhysicsBody& bodyB, const Vector2D& normal ) const {

    float inverseMassA = bodyA.getIsStatic() ? 0.0f : 1.0f / bodyA.getMass();
    float inverseMassB = bodyB.getIsStatic() ? 0.0f : 1.0f / bodyB.getMass();
    float inverseMassSum = inverseMassA + inverseMassB;

    if ( inverseMassSum <= Math::EPSILON ) return;

    Vector2D relativeVelocity = bodyB.getVelocity() - bodyA.getVelocity();

    float velocityAlongNormal = relativeVelocity.dotProduct( normal );

    if ( velocityAlongNormal >= 0.0f ) return;

    float restitution = std::min( bodyA.getMaterial().getRestitution(), bodyB.getMaterial().getRestitution() );
    float impulseMagnitude = -( 1.0f + restitution ) * velocityAlongNormal  / inverseMassSum;
    Vector2D impulse = normal * impulseMagnitude;

    bodyA.setVelocity( bodyA.getVelocity() - impulse * inverseMassA );
    bodyB.setVelocity( bodyB.getVelocity() + impulse * inverseMassB );
}
