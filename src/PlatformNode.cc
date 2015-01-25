#include "PlatformNode.h"

PlatformNode::PlatformNode(const sf::Texture& texture)
: SpriteNode(texture)
, mVisibility(true) {
    Utils::centerOrigin(mSprite);
}

void PlatformNode::changeVisibility() {
    mVisibility ^= true;
    if (mVisibility) {
        mBody->SetActive(true);
    }
    else {
        mBody->SetActive(false);
    }
}

void PlatformNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    if (mVisibility) {
        target.draw(mSprite, states);
    }
}

