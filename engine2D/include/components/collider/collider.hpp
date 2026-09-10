#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cmath>

struct Collider {

    Transform& transform;

    sf::Vector2<float> size;
    sf::Vector2<float> shape; // 0 = carrée; 1 = cercle

    


    Collider( Transform& transform, sf::Vector2<float> size = { 0.0f, 0.0f }, sf::Vector2<float> shape = { 0.0f, 0.0f } )
            : transform( transform ),
            size( size ),
            shape( shape )
    {
    }


    bool checkCollision( const Collider& otherCollider ) const;

private:

    sf::Vector2f support( sf::Vector2f direction ) const;

    static sf::Vector2f supportMinkowski( const Collider& a, const Collider& b, sf::Vector2f direction );

    static bool nextSimplex( std::vector<sf::Vector2f>& simplex, sf::Vector2f& direction );

};
