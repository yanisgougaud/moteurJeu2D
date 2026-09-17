#pragma once

#include "../transform/transform.hpp"
#include "../collider/collider.hpp"
#include "../renderer/renderer.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class Sprite {

    Transform& transform;
    Collider& collider;
    Renderer& renderer;
    sf::Texture texture;
    sf::Sprite sprite;

public:

    Sprite( Transform& transform, Collider& collider, Renderer& renderer, const std::string& spritePath )
    : transform( transform ),
      collider( collider ),
      renderer( renderer ),
      texture( spritePath ),
      sprite( texture )
    {
    }

    Sprite( Transform& transform, Collider& collider, Renderer& renderer )
    : transform( transform ),
      collider( collider ),
      renderer( renderer ),
      texture(),
      sprite( texture )
    {
    }

    const sf::Sprite& getSprite() const {
        return sprite;
    }

    sf::Sprite getSpriteForPrint() const;
};

