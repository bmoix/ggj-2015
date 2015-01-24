#include "AnimatedTextNode.h"

AnimatedTextNode::AnimatedTextNode(const sf::Font& font, const std::string& text) :
    TextNode(font, text) {
}

void AnimatedTextNode::setInitPosition(sf::Vector2f position) {
    mInitPosition = position;
}

void AnimatedTextNode::setFinalPosition(sf::Vector2f position) {
    mFinalPosition = position;
}

void AnimatedTextNode::setInitSize(float size) {
    mInitSize = size;
}

void AnimatedTextNode::setFinalSize(float size) {
    mFinalSize = size;
}

void AnimatedTextNode::setInitRotation(float rotation) {
    mInitRotation = rotation;
}

void AnimatedTextNode::setFinalRotation(float rotation) {
    mFinalRotation = rotation;
}

void AnimatedTextNode::setDuration(float seconds) {
    mDuration = seconds;
}

void AnimatedTextNode::initAnimation() {
    mClock.restart();
}
void AnimatedTextNode::updateCurrent(sf::Time dt) {
    float elapsedTime = mClock.getElapsedTime().asSeconds();
    sf::Vector2f posPerTime = (mFinalPosition - mInitPosition)/mDuration;
    sf::Vector2f newPosition = mInitPosition + posPerTime*elapsedTime;
    mText.setPosition(newPosition);
    

    float sizePerTime = (mFinalSize - mInitSize)/mDuration;
    float newSize = mInitSize + sizePerTime*elapsedTime;
    mText.setCharacterSize(newSize);

    float rotPerTime = (mFinalRotation - mInitRotation)/mDuration;
    float newRotation = mInitRotation + rotPerTime*elapsedTime;
    mText.rotate(newRotation);
}

