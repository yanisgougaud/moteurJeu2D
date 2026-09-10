#pragma once




struct Math {

    static float dotProduct( const sf::Vector2f a, const sf::Vector2f b ) const { 
        return a.x * b.x + a.y * b.y; 
    }

    static sf::Vector2f multiplication( const sf::Vector2f vector, const float value )const  {
        return { vector.x * value, vector.y * value };
    }

    static sf::Vector2f opposite( const sf::Vector2f vector )const {
        return { -vector.x, -vector.y };
    }

}

// ! Peut être mettre des référence en paramètre plutôt // Titouan