#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "Utils.h"
#include "SceneNode.h"

class SpriteNode: public SceneNode {
    public:
        explicit SpriteNode(const sf::Texture& texture);
        SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
        virtual void setSize(sf::Vector2u desiredSize);

        void setColor(sf::Color color);
        sf::Color getColor();

        // Create a body from the sprite
        virtual void createBody(b2World* world, bool dynamic);

    protected:
        b2Body* mBody;
        sf::Sprite mSprite;

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        // Setup the body definition and fixtures
        void setupBody(b2World* world, bool dynamic); 
};

#endif // SPRITE_NODE_HPP
