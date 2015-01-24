#ifndef PLAYER_H
#define PLAYER_H

#include "Utils.h"
#include "SceneNode.h"
#include "SpriteNode.h"

class Player: public SpriteNode {
    public:
        explicit Player(const sf::Texture& texture);

        bool isJumping();
        void isJumping(bool isJumping);

        void addVel(float x, float y);
        void setVel(float x, float y);
        sf::Vector2f getVel();
    private:
        void updateCurrent(sf::Time dt);

        bool mIsJumping;

        sf::Vector2f mVelocity;
        sf::Vector2f mAcceleration;
};

#endif // PLAYER_H
