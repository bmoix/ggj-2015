#ifndef ANIMATION_NODE_HPP
#define ANIMATION_NODE_HPP

#include "Utils.h"
#include "SpriteNode.h"

class AnimationNode: public SpriteNode {
    struct AnimFrame {
        sf::Vector2i mPos;
        sf::Vector2i mSize;
        float mTime;
    };

    public:
        explicit AnimationNode(const sf::Texture& texture, const std::string& file);
        AnimationNode(const sf::Texture& texture, const sf::IntRect& textureRect, const std::string& file);
        void setSize(sf::Vector2u desiredSize) override;
        void setAnimation(const std::string& animation);


    private:
        void updateCurrent(sf::Time dt) override;
        void load(const std::string& filename);
        void updateTextureRect();

        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;      

        std::map<std::string, std::vector<AnimFrame>> mAnimations;
        int mCurrentFrame;
        std::string mCurrentAnim;
        float mCurrentTime;
        sf::Vector2u mSize;
};

#endif // ANIMATION_NODE_HPP
