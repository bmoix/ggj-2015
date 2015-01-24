#ifndef ANIMATED_TEXT_NODE_HPP
#define ANIMATED_TEXT_NODE_HPP

#include "Utils.h"
#include "TextNode.h"

class AnimatedTextNode: public TextNode {
	public:
		explicit AnimatedTextNode(const sf::Font& font, const std::string& text);
    void setInitPosition(sf::Vector2f position);
    void setFinalPosition(sf::Vector2f position);
    void setInitSize(float size);
    void setFinalSize(float size);
    void setInitRotation(float rotation);
    void setFinalRotation(float rotation);
    void setInitColor(sf::Color color);
    void setFinalColor(sf::Color color);
    void setDuration(float seconds);
    void initAnimation();
    void updateCurrent(sf::Time dt) override;

	private:
    sf::Vector2f mInitPosition;
    sf::Vector2f mFinalPosition;
    float mInitSize;
    float mFinalSize;
    float mInitRotation;
    float mFinalRotation;
    sf::Color mInitColor;
    sf::Color mFinalColor;
    float mDuration;
    sf::Clock mClock;
};

#endif // ANIMATED_TEXT_NODE_HPP
