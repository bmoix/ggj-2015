#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "Utils.h"
#include "SceneNode.h"
#include "Collision.h"

class SpriteNode: public SceneNode {
    public:
        explicit SpriteNode(const sf::Texture& texture);
        explicit SpriteNode(const sf::Texture& texture, CollisionType type);
        SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
        virtual void setSize(sf::Vector2u desiredSize);

        void setColor(sf::Color color);
        sf::Color getColor();
        sf::IntRect getBounds();

        CollisionType getCollisionType() const;
        void setCollisionType(CollisionType type);

        // Create a body from the sprite
        virtual void createBody(b2World* world, bool dynamic, float bbscalex = 1.0f, float bbscaley = 1.0f, float density = 1.0f, float rotation = 0.0f);
        virtual void collidedWith(SpriteNode* other, b2Vec2 normal);
        virtual void endContactWith(SpriteNode* other, b2Vec2 normal);
        virtual b2Body* createSphericBody(b2World* world, bool dynamic, float bbscale, float density);

        sf::Vector2f mSize;

    protected:
        b2Body* mBody;
        sf::Sprite mSprite;
        CollisionType mCollisionType;
        
        // Setup the body definition and fixtures
        void setupBody(b2World* world, bool dynamic, float bbscalex=1.0f, float bbscaley = 1.0f, float density = 1.0f, float rotation = 0.0f); 

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time dt) override;
};

#endif // SPRITE_NODE_HPP
