#include "include.hpp"

#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>



// Changer la mass change la direction

int main()
{

    try {

        sf::RenderWindow window(
            sf::VideoMode({800, 600}),
            "Moteur 2D"
        );

        sf::CircleShape circle(100.f);

        circle.setFillColor(sf::Color::Blue);

        circle.setPosition({
            300.f,
            200.f
        });


// 750 579

        Renderer renderer( window );


        Transform transform(  { 0.0f, 500.0f }, { 0.2f, 0.2f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, 0.0f, 0.0f );
        Collider collider( transform, { {0.0f, 0.0f }, { 0.0f, 579.0f }, { 750.0f, 579.0f }, { 750.0f, 0.0f } } );
        Sprite sprite( transform, collider, renderer, "../assets/test.png" );
        Material material( 0.5f, 0.0f );
        PhysicsBody physicsBody( transform, collider, material, 10.0f, 1.0f, 1.0f, false );


        Transform transform2(  { 0.0f, 400.0f }, { 1.0f, 0.1f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, 0.0f, 0.0f );
        Collider collider2( transform2, { {0.0f, 0.0f }, { 0.0f, 579.0f }, { 750.0f, 579.0f }, { 750.0f, 0.0f } } );
        Sprite sprite2( transform2, collider2, renderer, "../assets/test.png" );
        Material material2( 0.5f, 0.0f );
        PhysicsBody physicsBody2( transform2, collider2, material2, 1.0f, 1.0f, 1.0f, true );



        PhysicsWorld physicsWorld( { 0.0f, -9.81f }, 1.0f, { 3.0f, 9.0f }, 1.225f, 120.0f );
        physicsWorld.addBody( physicsBody );
        physicsWorld.addBody( physicsBody2 );

        sf::Clock clock;

        float accumulator = 0.0f;


        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
            }

            float deltaTime = clock.restart().asSeconds();

            accumulator += deltaTime;

            // Simulation physique à pas fixe
            while (accumulator >= physicsWorld.getFixedDeltaTime())
            {
                physicsWorld.step();

                accumulator -= physicsWorld.getFixedDeltaTime();
            }

            window.clear(sf::Color::Black);

            window.draw(circle);

            window.draw ( sprite.getSpriteForPrint() );
            window.draw ( sprite2.getSpriteForPrint() );

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