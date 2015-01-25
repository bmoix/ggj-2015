#include "Player.h"

Player::Player(const sf::Texture& texture, const std::string& file) : 
    AnimationNode(texture, file),
    mState(Player::States::IdleRight),
    mDoubleJumpUsed(false),
    mLookingRight(true),
    mDead(false),
    mInGround(0),
    mInWall(0) {
}

void Player::updateCurrent(sf::Time dt) {
    AnimationNode::updateCurrent(dt);
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

bool Player::isDead() const {
    return mDead;
}

Player::States Player::getState() {
    return mState;
}

void Player::setState(Player::States state) {
    mState = state;
}

void Player::updateState() {
    if (mDead) {
        if (mLookingRight) setState(States::DeadRight);
        else setState(States::DeadLeft);
        return;
    }

    b2Vec2 v = mBody->GetLinearVelocity();
    sf::Vector2f mVelocity(v.x, v.y);

    if (abs(mVelocity.x) < epsilon && abs(mVelocity.y) < epsilon) {
        if (mInWall && !mInGround) {
            if (mLookingRight) setState(Player::States::WallRight);
            else setState(Player::States::WallLeft);
        }
        else {
            if (mLookingRight) setState(Player::States::IdleRight);
            else setState(Player::States::IdleLeft);
        }
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
        case States::DeadRight:
            setAnimation("DeadRight");
            break;
        case States::DeadLeft:
            setAnimation("DeadLeft");
            break;
        case States::WallRight:
            setAnimation("WallRight");
            break;
        case States::WallLeft:
            setAnimation("WallLeft");
            break;
        default:
            break;
    }
}

void Player::createBody(b2World* world, bool dynamic, float bbscalex, float bbscaley, float density, float rotation) {
    setupBody(world, dynamic, bbscalex, bbscaley, density);
    mBody->SetUserData(this);
    mBody->SetFixedRotation(true);
}

void Player::collidedWith(SpriteNode* other, b2Vec2 normal) {
    if (other) {
        if (normal.y > 0) mDoubleJumpUsed = false;
        CollisionType type = other->getCollisionType();
        if (type == CollisionType::SpikesBall ||
            (type == CollisionType::Spikes && 
            (normal.y > 0 || std::abs(normal.x) > epsilon))) {
            mDead = true;
            setVel(0, -1000);
        }
        if (type == CollisionType::Ground) {
            ++mInGround;
            mDoubleJumpUsed = false;
        }
        if (std::abs(normal.x) > epsilon && type == CollisionType::Wall) {
            ++mInWall;
            mDoubleJumpUsed = false;
        }
    }
}

void Player::endContactWith(SpriteNode* other, b2Vec2 normal) {
    if (other) {
        CollisionType type = other->getCollisionType();
        if (type == CollisionType::Ground) {
            mInGround = 0;
        }
        if (type == CollisionType::Wall) {
            mInWall = 0;
        }
    }
}
