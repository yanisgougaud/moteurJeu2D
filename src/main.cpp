#include "include.hpp"

#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>


int main()
{

    try {

        sf::RenderWindow window(
            sf::VideoMode({800, 600}),
            "Moteur 2D"
        );

        sf::CircleShape circle(100.f);

        circle.setFillColor(sf::Color::Red);

        circle.setPosition({
            300.f,
            200.f
        });

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
            }

            window.clear(sf::Color::Black);

            window.draw(circle);

            window.display();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERREUR : " << e.what() << '\n';

        std::cin.get();
        return 1;
    }

    return 0;
}