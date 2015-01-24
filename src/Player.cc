#include "Player.h"

Player::Player(const sf::Texture& texture, const std::string& file) : 
    AnimationNode(texture, file),
    mVelocity(sf::Vector2f(0.0f,0.0f)),
    mAcceleration(sf::Vector2f(0.0f,3000.0f)),
    mState(Player::States::Idle),
    mDoubleJumpUsed(false) {
}

void Player::updateCurrent(sf::Time dt) {
    AnimationNode::updateCurrent(dt);
    float dtime = dt.asSeconds();
    Player::States oldState = mState;
    updateState();
    if (mState != oldState) {
        changeAnimation();
    }
	//float dtime = dt.asSeconds();
	//float dx = mVelocity.x*dtime + 0.5*mAcceleration.x*dtime*dtime;
	//float dy = mVelocity.y*dtime + 0.5*mAcceleration.y*dtime*dtime;
	//mVelocity.x += mAcceleration.x*dtime;
	//mVelocity.y += mAcceleration.y*dtime;
	//mSprite.move(dx,dy);
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
    if (abs(mVelocity.x) < 1e-4 && abs(mVelocity.y) < 1e-4) {
        setState(Player::States::Idle);
        mDoubleJumpUsed = false;
    }
    else if (mVelocity.x > 0 && abs(mVelocity.y) < 1e-4) {
        setState(Player::States::Right);
        mDoubleJumpUsed = false;
    }
    else if (mVelocity.x < 0 && abs(mVelocity.y) < 1e-4) {
        setState(Player::States::Left);
        mDoubleJumpUsed = false;
    }
    else if (mVelocity.x >= 0 && mVelocity.y < 0) {
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

void Player::createBody(b2World* world, bool dynamic, float bbscale) {
    setupBody(world, dynamic, bbscale);
    mBody->SetUserData(this);
    mBody->SetFixedRotation(true);
}

void Player::collidedWith(SpriteNode* other) {
    if (other) {
        if (other->mType == 1) std::cout << "Left wall" << std::endl;
        else if (other->mType == 2) std::cout << "Right wall" << std::endl;
    }
}
