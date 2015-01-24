#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "Utils.h"
#include "SceneNode.h"
#include "ObjectTypeIdentifiers.h"

class SpriteNode: public SceneNode {
    public:
        explicit SpriteNode(const sf::Texture& texture, ObjectTypes::ID type);
        SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect, ObjectTypes::ID type);
        virtual void setSize(sf::Vector2u desiredSize);

        ObjectTypes::ID getType();

        virtual bool canCollideWith(ObjectTypes::ID type);
        virtual void hasCollidedWith(SpriteNode* other);

        void setColor(sf::Color color);
        sf::Color getColor();

    protected:
        sf::Sprite mSprite;
        ObjectTypes::ID mType;

    private:

        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;      
};

#endif // SPRITE_NODE_HPP
