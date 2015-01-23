#ifndef PLAYER_H
#define PLAYER_H

#include "Utils.h"
#include "SceneNode.h"
#include "SpriteNode.h"

class Player: public SpriteNode {
    public:
        explicit Player(const sf::Texture& texture);


    private:
        void updateCurrent(sf::Time dt);
};

#endif // PLAYER_H
