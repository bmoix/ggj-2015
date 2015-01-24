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
        sf::IntRect getBounds();

        // Create a body from the sprite
        virtual void createBody(b2World* world, bool dynamic);

    protected:
        b2Body* mBody;
        sf::Sprite mSprite;
        sf::Vector2f mSize;
        // Setup the body definition and fixtures
        void setupBody(b2World* world, bool dynamic); 

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time dt) override;
};

#endif // SPRITE_NODE_HPP
