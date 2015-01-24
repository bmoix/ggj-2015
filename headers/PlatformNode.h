#ifndef PLATFORM_NODE_H
#define PLATFORM_NODE_H

#include "Utils.h"
#include "SpriteNode.h"

class PlatformNode : public SpriteNode {
    public:
        explicit PlatformNode(const sf::Texture& texture);
        void changeVisibility();

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        bool mVisibility;
};

#endif // PLATFORM_NODE_H
