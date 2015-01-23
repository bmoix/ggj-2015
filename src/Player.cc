#include "Player.h"

Player::Player(const sf::Texture& texture)
: SpriteNode(texture) {

}   

void Player::updateCurrent(sf::Time dt) {
	mSprite.move(10.0f,0.0f);
}