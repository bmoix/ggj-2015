#ifndef INVISIBLE_NODE_HPP
#define INVISIBLE_NODE_HPP

#include "Utils.h"
#include "SpriteNode.h"

class InvisibleNode: public SpriteNode {
    public:
        explicit InvisibleNode(const sf::Texture& texture);

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // INVISIBLE_NODE_HPP
