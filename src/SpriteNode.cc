#include "SpriteNode.h"

SpriteNode::SpriteNode(const sf::Texture& texture)
: mSprite(texture) {
}   

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect) : mSprite(texture, textureRect) {
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}

void SpriteNode::setSize(sf::Vector2u desiredSize){
    float scalex, scaley;
    scalex = scaley = 0.0;
    scalex = float(desiredSize.x)/mSprite.getTexture()->getSize().x;
    scaley = float(desiredSize.y)/mSprite.getTexture()->getSize().y;
    setScale(scalex, scaley);
}

void SpriteNode::setColor(sf::Color color){
    mSprite.setColor(color);
}

sf::Color SpriteNode::getColor(){
    return mSprite.getColor();
}