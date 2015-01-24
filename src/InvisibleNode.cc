#include "InvisibleNode.h"

InvisibleNode::InvisibleNode(const sf::Texture& texture) : 
    SpriteNode(texture) {
}   

void InvisibleNode::collidedWith(SpriteNode* other) {

}

void InvisibleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
}
