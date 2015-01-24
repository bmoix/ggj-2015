#include "Player.h"

Player::Player(const sf::Texture& texture)
: SpriteNode(texture),
mIsJumping(false),
mVelocity(sf::Vector2f(0.0f,0.0f)),
mAcceleration(sf::Vector2f(0.0f,1500.0f)) {

}

void Player::updateCurrent(sf::Time dt) {
	float dtime = dt.asSeconds();
	float dx = mVelocity.x*dtime + 0.5*mAcceleration.x*dtime*dtime;
	float dy = mVelocity.y*dtime + 0.5*mAcceleration.y*dtime*dtime;
	mVelocity.x += mAcceleration.x*dtime;
	mVelocity.y += mAcceleration.y*dtime;
	mSprite.move(dx,dy);
}

bool Player::isJumping(){
	return mIsJumping;
}

void Player::isJumping(bool isJumping){
	mIsJumping = isJumping;
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