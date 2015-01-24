#ifndef PLAYER_H
#define PLAYER_H

#include "Utils.h"
#include "SceneNode.h"
#include "AnimationNode.h"

class Player: public AnimationNode {
    public:
        explicit Player(const sf::Texture& texture, const std::string& file);

        enum class States {
            Idle,
            Left,
            Right,
            JumpLeft,
            JumpRight,
            FallLeft,
            FallRight,
            Dying
        };

        Player::States getState();
        void setState(Player::States state);

        void addVel(float x, float y);
        void setVel(float x, float y);
        void scaleVel(float x, float y);
        sf::Vector2f getVel();
        bool isJumping() const;
        bool canJump() const;
        void jump(float v);

        virtual void createBody(b2World* world, bool dynamic, float bbscalex = 1.0f, float bbscaley = 1.0f) override;
        virtual void collidedWith(SpriteNode* other) override;

    private:
        void updateCurrent(sf::Time dt);
        void updateState();
        void changeAnimation();

        States mState;

        sf::Vector2f mVelocity;
        sf::Vector2f mAcceleration;

        bool mDoubleJumpUsed;
};

#endif // PLAYER_H
