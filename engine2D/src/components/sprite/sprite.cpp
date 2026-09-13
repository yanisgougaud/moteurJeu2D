#include "../../../include/components/sprite/sprite.hpp"

#include <cmath>

sf::Sprite Sprite::getSpriteForPrint() const {

    sf::Sprite spriteForPrint = sprite;

    const sf::Vector2u textureSize = texture.getSize();

    spriteForPrint.setOrigin( { 0.0f, static_cast<float>( textureSize.y ) } );

    Vector2D imagePosition = { -transform.getOriginX(), -transform.getOriginY() };

    imagePosition.x *= transform.getScaleX();
    imagePosition.y *= transform.getScaleY();

    Vector2D centerOfGravity = { transform.getCenterOfGravityX() - transform.getOriginX(), transform.getCenterOfGravityY() - transform.getOriginY() };

    centerOfGravity.x *= transform.getScaleX();
    centerOfGravity.y *= transform.getScaleY();

    Vector2D relativePosition = imagePosition - centerOfGravity;

    const float cosAngle = std::cos(transform.getRadianAngle());
    const float sinAngle = std::sin(transform.getRadianAngle());

    Vector2D rotatedPosition = {
        relativePosition.x * cosAngle - relativePosition.y * sinAngle,
        relativePosition.x * sinAngle + relativePosition.y * cosAngle
    };

    Vector2D finalPosition = rotatedPosition + centerOfGravity;

    finalPosition += transform.getPosition();

    spriteForPrint.setPosition( { finalPosition.x, renderer.getWindow().getSize().y - finalPosition.y } );

    spriteForPrint.setRotation( sf::degrees( transform.getRotation() ) );

    spriteForPrint.setScale( { transform.getScaleX(), transform.getScaleY() } );

    return spriteForPrint;

}
