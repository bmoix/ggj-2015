#include "AnimatedTextNode.h"

AnimatedTextNode::AnimatedTextNode(const sf::Font& font, const std::string& text) :
    TextNode(font, text),
    mInitPosition(1920.0f/2.0f, 1080.0f/2.0f),
    mFinalPosition(mInitPosition),
    mInitSize(10.0f),
    mFinalSize(mInitSize),
    mInitRotation(0.0f),
    mFinalRotation(0.0f),
    mInitColor(sf::Color::Black), 
    mFinalColor(mInitColor) {
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

void AnimatedTextNode::setInitColor(sf::Color color) {
    mInitColor = color;
}

void AnimatedTextNode::setFinalColor(sf::Color color) {
    mFinalColor = color;
}

void AnimatedTextNode::setDuration(float seconds) {
    mDuration = seconds;
}

void AnimatedTextNode::initAnimation() {
    mClock.restart();
}
void AnimatedTextNode::updateCurrent(sf::Time dt) {
    (void) dt;
    float elapsedTime = mClock.getElapsedTime().asSeconds();
    if (elapsedTime > mDuration) return;

    sf::Vector2f posPerTime = (mFinalPosition - mInitPosition)/mDuration;
    sf::Vector2f newPosition = mInitPosition + posPerTime*elapsedTime;
    mText.setPosition(newPosition);
    
    float sizePerTime = (mFinalSize - mInitSize)/mDuration;
    float newSize = mInitSize + sizePerTime*elapsedTime;
    mText.setCharacterSize(newSize);

    float rotPerTime = (mFinalRotation - mInitRotation)/mDuration;
    float newRotation = mInitRotation + rotPerTime*elapsedTime;
    mText.setRotation(newRotation);

    float r = (mFinalColor.r - mInitColor.r)/mDuration;
    float g = (mFinalColor.g - mInitColor.g)/mDuration;
    float b = (mFinalColor.b - mInitColor.b)/mDuration;
    sf::Color newColor(
        sf::Uint8(mInitColor.r + r*elapsedTime),
        sf::Uint8(mInitColor.g + g*elapsedTime),
        sf::Uint8(mInitColor.b + b*elapsedTime)
    );
    mText.setColor(newColor);
    centerText();
}

