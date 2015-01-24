#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "Utils.h"
#include "SceneNode.h"

class SpriteNode: public SceneNode {
    public:
        explicit SpriteNode(const sf::Texture& texture);
        SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
        virtual void setSize(sf::Vector2u desiredSize);
        void setPosition(const sf::Vector2f &position);
        void setSize(sf::Vector2u desiredSize); 
        void setColor(sf::Color color);
        sf::Color getColor();

    protected:
        sf::Sprite mSprite;

    private:

        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;      
};

#endif // SPRITE_NODE_HPP
