#pragma once

#include <stdexcept>

#include "../../math/Vector2D.hpp"
#include "../collider/collider.hpp"
#include "../material/material.hpp"
#include "../transform/transform.hpp"
#include "../input/input.hpp"

class PhysicsBody {

    Transform& transform;
    Collider& collider;
    Material& material;
    Input& input;
    float mass;
    Vector2D velocity;
    Vector2D acceleration;
    Vector2D force;
    float dragCoefficient;
    float aerodynamicDepth;
    float accelerationRelaxation;
    float decelerationRelaxation;
    bool isStatic;

public:

    PhysicsBody(
        Transform& transform,
        Collider& collider,
        Material& material,
        Input& input,
        float mass = 1.0f,
        float dragCoefficient = 1.0f,
        float aerodynamicDepth = 1.0f,
        float accelerationRelaxation = 0.001f,
        float decelerationRelaxation = 0.01f,
        bool isStatic = false
    )
        : transform( transform ),
          collider( collider ),
          material( material ),
          input( input ),
          velocity( { 0.0f, 0.0f } ),
          acceleration( { 0.0f, 0.0f } ),
          force( { 0.0f, 0.0f } ),
          dragCoefficient( dragCoefficient ),
          aerodynamicDepth( aerodynamicDepth ),
          accelerationRelaxation( accelerationRelaxation ),
          decelerationRelaxation( decelerationRelaxation ),
          isStatic( isStatic )
    {
        setMass( mass );
        setDragCoefficient(dragCoefficient);
        setAerodynamicDepth(aerodynamicDepth);
    }

    Transform& getTransform() const {
        return transform;
    }

    Collider& getCollider() const {
        return collider;
    }

    void setMaterial( const Material& newMaterial ) {
        material = newMaterial;
    }

    const Material& getMaterial() const {
        return material;
    }

    void setInput( const Input& newInput ) {
        input = newInput;
    }

    const Input& getInput() const {
        return input;
    }

    void setMass( const float newMass ) {

        if ( newMass <= 0.0f ) throw std::runtime_error( "La masse doit etre strictement positive." );
        mass = newMass;
    }

    float getMass() const {
        return mass;
    }

    void setVelocity( const Vector2D& newVelocity ) {
        velocity = newVelocity;
    }

    const Vector2D& getVelocity() const {
        return velocity;
    }

    void setAcceleration( const Vector2D& newAcceleration ) {
        acceleration = newAcceleration;
    }

    const Vector2D& getAcceleration() const {
        return acceleration;
    }

    void setForce( const Vector2D& newForce ) {
        force = newForce;
    }

    const Vector2D& getForce() const {
        return force;
    }

    void addForce( const Vector2D& newForce ) {
        force += newForce;
    }

    void clearForces() {
        force = { 0.0f, 0.0f };
    }

    void setStatic( const bool newIsStatic ) {
        isStatic = newIsStatic;
    }

    void setDragCoefficient( const float newDragCoefficient ) {

        if ( newDragCoefficient < 0.0f ) throw std::runtime_error( "Le coefficient de trainee doit être strictement positive." );
        dragCoefficient = newDragCoefficient;
    }

    float getDragCoefficient() const {
        return dragCoefficient;
    }

    void setAerodynamicDepth( const float newAerodynamicDepth ) {

        if ( newAerodynamicDepth <= 0.0f ) throw std::runtime_error( "La profondeur aerodynamique doit être strictement positive." );
        aerodynamicDepth = newAerodynamicDepth;
    }

    float getAerodynamicDepth() const {
        return aerodynamicDepth;
    }

    float getAccelerationRelaxation() const {
        return accelerationRelaxation;
    }

    void setAccelerationRelaxation( float newAccelerationRelaxation ) {
        if ( newAccelerationRelaxation <= 0.0f ) throw std::runtime_error( "La relaxation de l'accélération doit être strictement positive." );
        accelerationRelaxation = newAccelerationRelaxation;
    }

    float getDecelerationRelaxation() const {
        return decelerationRelaxation;
    }

    void setIDecelerationRelaxation(float newDecelerationRelaxation ) {
        if ( newDecelerationRelaxation <= 0.0f ) throw std::runtime_error( "La relaxation de la décélération doit être strictement positive." );
        decelerationRelaxation = newDecelerationRelaxation;
    }

    bool getIsStatic() const {
        return isStatic;
    }

    void setIsStatic(bool newIsStatic ) {
        isStatic = newIsStatic;
    }
};

