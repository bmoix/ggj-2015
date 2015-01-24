#include "Player.h"

Player::Player(const sf::Texture& texture, const std::string& file) : 
    AnimationNode(texture, file),
    mVelocity(sf::Vector2f(0.0f,0.0f)),
    mAcceleration(sf::Vector2f(0.0f,3000.0f)),
    mState(Player::States::IdleRight),
    mDoubleJumpUsed(false),
    mLookingRight(true) {
}

void Player::updateCurrent(sf::Time dt) {
    AnimationNode::updateCurrent(dt);
    float dtime = dt.asSeconds();
    Player::States oldState = mState;
    updateState();
    if (mState != oldState) {
        changeAnimation();
    }
    if (mBody != NULL) {
        b2Vec2 pos = mBody->GetPosition();
        setPosition(pos.x*metersToPixels, pos.y*metersToPixels);
    }
}

void Player::addVel(float x, float y) {
}

void Player::setVel(float x, float y) {
    b2Vec2 velocity = mBody->GetLinearVelocity();
    mBody->SetLinearVelocity(b2Vec2(x/metersToPixels, y/metersToPixels+velocity.y));
}

void Player::scaleVel(float x, float y) {
    b2Vec2 velocity = mBody->GetLinearVelocity();
    mBody->SetLinearVelocity(b2Vec2(velocity.x * x, velocity.y * y));
}

sf::Vector2f Player::getVel() {
    return mVelocity;
}

bool Player::isJumping() const {
    if (mState == States::JumpLeft ||
        mState == States::JumpRight ||
        mState == States::FallLeft ||
        mState == States::FallRight) {
        return true;
    }
    return false;
}

bool Player::canJump() const {
    switch(mState) {
        case States::JumpLeft:
        case States::JumpRight:
        case States::FallLeft:
        case States::FallRight:
            if (mDoubleJumpUsed) return false;
            break;
    }
    return true;
}

void Player::jump(float v) {
    scaleVel(1.0f, 0.0f);
    setVel(0.0f, v);
    if (isJumping()) {
        scaleVel(1.0f, 0.7f); // Reduce double jump velocity
    }
    switch(mState) {
        case States::JumpLeft:
        case States::JumpRight:
        case States::FallLeft:
        case States::FallRight:
            mDoubleJumpUsed = true;
            break;
    }
}

Player::States Player::getState() {
    return mState;
}

void Player::setState(Player::States state) {
    mState = state;
}

void Player::updateState() {
    b2Vec2 v = mBody->GetLinearVelocity();
    mVelocity = sf::Vector2f(v.x, v.y);

    if (abs(mVelocity.x) < epsilon && abs(mVelocity.y) < epsilon) {
        if (mLookingRight) setState(Player::States::IdleRight);
        else setState(Player::States::IdleLeft);
    }
    else if (abs(mVelocity.y) < epsilon) {
        if (mLookingRight) setState(Player::States::Right);
        else setState(Player::States::Left);
    }
    else if (mVelocity.y < 0) {
        if (mLookingRight) setState(Player::States::JumpRight);
        else setState(Player::States::JumpLeft);
    }
    else if (mVelocity.y > 0) { 
        if (mLookingRight) setState(Player::States::FallRight);
        else setState(Player::States::FallLeft);
    }
}

void Player::setLookingRight(bool state) {
    mLookingRight = state;
}

void Player::changeAnimation() {
    switch(mState) {
        case States::IdleRight:
            setAnimation("IdleRight");
            break;
        case States::IdleLeft:
            setAnimation("IdleLeft");
            break;
        case States::JumpRight:
            setAnimation("JumpRight");
            break;
        case States::JumpLeft:
            setAnimation("JumpLeft");
            break;
        case States::FallRight:
            setAnimation("FallRight");
            break;
        case States::FallLeft:
            setAnimation("FallLeft");
            break;
        case States::Right:
            setAnimation("Right");
            break;
        case States::Left:
            setAnimation("Left");
            break;
        default:
            break;
    }
}

void Player::createBody(b2World* world, bool dynamic, float bbscalex, float bbscaley) {
    setupBody(world, dynamic, bbscalex, bbscaley);
    mBody->SetUserData(this);
    mBody->SetFixedRotation(true);
}

void Player::collidedWith(SpriteNode* other, b2Vec2 normal) {
    if (other) {
        if (normal.y > 0) mDoubleJumpUsed = false;

        if (other->mType == 1) std::cout << "Left wall" << std::endl;
        else if (other->mType == 2) std::cout << "Right wall" << std::endl;
    }
}
