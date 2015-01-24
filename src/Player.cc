#include "Player.h"

Player::Player(const sf::Texture& texture, const std::string& file)
: AnimationNode(texture, file),
mVelocity(sf::Vector2f(0.0f,0.0f)),
mAcceleration(sf::Vector2f(0.0f,3000.0f)),
mState(Player::States::Idle) {

}

void Player::updateCurrent(sf::Time dt) {
    AnimationNode::updateCurrent(dt);
    float dtime = dt.asSeconds();
    float dx = mVelocity.x*dtime + 0.5*mAcceleration.x*dtime*dtime;
    float dy = mVelocity.y*dtime + 0.5*mAcceleration.y*dtime*dtime;
    mVelocity.x += mAcceleration.x*dtime;
    mVelocity.y += mAcceleration.y*dtime;
    Player::States oldState = mState;
    updateState();
    if (mState != oldState) {
        changeAnimation();
    }
    mSprite.move(dx,dy);
}

void Player::addVel(float x, float y) {
    mVelocity.x += x;
    mVelocity.y += y;
}

void Player::setVel(float x, float y) {
    mVelocity.x = x;
    mVelocity.y = y;
}

sf::Vector2f Player::getVel() {
    return mVelocity;
}

Player::States Player::getState() {
    return mState;
}

void Player::setState(Player::States state) {
    mState = state;
}

void Player::updateState() {
    if (abs(mVelocity.x) < 1e-6 && abs(mVelocity.y) < 1e-6) {
        setState(Player::States::Idle);
    }
    else if (mVelocity.x > 0 && abs(mVelocity.y) < 1e-6) {
        setState(Player::States::Right);
    }
    else if (mVelocity.x < 0 && abs(mVelocity.y) < 1e-6) {
        setState(Player::States::Left);
    }
    else if (mVelocity.x > 0 && mVelocity.y < 0) {
        setState(Player::States::JumpRight);
    }
    else if (mVelocity.x < 0 && mVelocity.y < 0) {
        setState(Player::States::JumpLeft);
    }
    else if (mVelocity.x > 0 && mVelocity.y > 0) {
        setState(Player::States::FallRight);
    }
    else if (mVelocity.x < 0 && mVelocity.y > 0) {
        setState(Player::States::FallLeft);
    }
    
}

void Player::changeAnimation() {
    switch(mState) {
        case States::Idle:
            setAnimation("idle");
            break;
        case States::JumpRight:
            setAnimation("jumpright");
            break;
        case States::JumpLeft:
            setAnimation("jumpleft");
            break;
        case States::FallRight:
            setAnimation("fallright");
            break;
        case States::FallLeft:
            setAnimation("fallleft");
            break;
        case States::Right:
            setAnimation("right");
            break;
        case States::Left:
            setAnimation("left");
            break;
        default:
            break;
    }
}