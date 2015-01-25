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
        void resetAnimation();
        float getElapsedTime() const;

        virtual void createBody(b2World* world, bool dynamic, float bbscalex=1.0f, float bbscaley = 1.0f, float density = 1.0f, float rotation = 0.0f) override;

    protected:
        void updateCurrent(sf::Time dt) override;
        
    private:
        
        void load(const std::string& filename);
        void updateTextureRect();

        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;      

        std::map<std::string, std::vector<AnimFrame>> mAnimations;
        std::map<std::string, unsigned int> mAnimationCycles;
        unsigned int mCurrentFrame;
        unsigned int mCurrentCycle;
        std::string mCurrentAnim;
        float mCurrentTime;
        float mTotalTime;
};

#endif // ANIMATION_NODE_HPP
