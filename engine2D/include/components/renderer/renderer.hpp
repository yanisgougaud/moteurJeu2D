#pragma once

#include <SFML/Graphics.hpp>

class Renderer {

    sf::RenderWindow& window;


public:

    Renderer( sf::RenderWindow& window )
    :  window( window )
    {
    }

    const sf::RenderWindow& getWindow() const {
        return window;
    }

};
