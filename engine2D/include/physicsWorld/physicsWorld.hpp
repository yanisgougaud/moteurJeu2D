#pragma once

#include <stdexcept>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../components/collider/collider.hpp"
#include "../components/physicsBody/physicsBody.hpp"

class PhysicsWorld {

    Vector2D gravity;
    float entityGravityScale;
    Vector2D wind;
    float airDensity;
    std::vector<PhysicsBody*> bodies;
    float physicsFPS;
    float fixedDeltaTime;
    Vector2D screenSize;
    const sf::RenderWindow& window;

    float calculateAerodynamicArea( const PhysicsBody& body, const Vector2D& direction ) const;

    Vector2D calculateRelativeAirVelocity( const PhysicsBody& body ) const {
        return wind - body.getVelocity();
    }

    Vector2D calculateDragForce( const PhysicsBody& body ) const {
        return calculateDragForce( body, body.getVelocity() );
    }

    Vector2D calculateDragForce( const PhysicsBody& body, const Vector2D& velocity ) const;

    Vector2D solveImplicitVelocity( const PhysicsBody& body, const Vector2D& otherForces ) const;
    void constrainToScreen( PhysicsBody& body );

public:

    PhysicsWorld(
        const sf::RenderWindow& window,
        const Vector2D& gravity = { 0.0f, -9.81f },
        float entityGravityScale = 1.0f,
        const Vector2D& wind = { 0.0f, 0.0f },
        float airDensity = 1.225f,
        float physicsFPS = 120.0f
    )
        : gravity( gravity ),
          entityGravityScale( entityGravityScale ),
          wind( wind ),
          airDensity( airDensity ),
                    physicsFPS( physicsFPS ),
          screenSize( { 0.0f, 0.0f } ),
          window( window )
    {
        setPhysicsFPS( physicsFPS );
    }


    void setGravity( const Vector2D& newGravity ) {
        gravity = newGravity;
    }

    const Vector2D& getGravity() const {
        return gravity;
    }

    void setEntityGravityScale( const float newScale ) {

        if ( newScale < 0.0f ) throw std::runtime_error( "Le facteur de gravité des entites doit être positif." );
        entityGravityScale = newScale;
    }

    float getEntityGravityScale() const {
        return entityGravityScale;
    }

    void setWind( const Vector2D& newWind ) {
        wind = newWind;
    }

    const Vector2D& getWind() const {
        return wind;
    }

    void setAirDensity( const float newAirDensity ) {

        if ( newAirDensity < 0.0f ) throw std::runtime_error( "La densité de l'air doit être strictement positive." );

        airDensity = newAirDensity;
    }

    float getAirDensity() const {
        return airDensity;
    }

    void setPhysicsFPS( const float newPhysicsFPS ) {

        if ( newPhysicsFPS <= 0.0f ) throw std::runtime_error( "Le FPS physique doit être strictement positif." );
        physicsFPS = newPhysicsFPS;
        fixedDeltaTime = 1.0f / physicsFPS;
    }

    float getPhysicsFPS() const {
        return physicsFPS;
    }

    float getFixedDeltaTime() const {
        return fixedDeltaTime;
    }

    void updateScreenSize() {

        const sf::Vector2u size = window.getSize();
        screenSize = {
            static_cast<float>( size.x ),
            static_cast<float>( size.y )
        };

        if ( screenSize.x <= 0.0f || screenSize.y <= 0.0f )
            throw std::runtime_error( "La taille de l'ecran doit être strictement positive." );
    }

    const Vector2D& getScreenSize() const {
        return screenSize;
    }

    void addBody( PhysicsBody& body ) {
        bodies.push_back( &body );
    }

    void removeBody( PhysicsBody& body );

    const std::vector<PhysicsBody*>& getBodies() const {
        return bodies;
    }

    void step();
};
