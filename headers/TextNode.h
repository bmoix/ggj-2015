#ifndef TEXT_NODE_HPP
#define TEXT_NODE_HPP

#include "Utils.h"
#include "SceneNode.h"

class TextNode: public SceneNode {
    public:
        explicit TextNode(const sf::Font& font, const std::string& text);
        void setString(const std::string& text);
        void setCharacterSize(unsigned int size);
        void setColor(const sf::Color& color);
        void centerText();

    protected:
        sf::Text mText;

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // TEXT_NODE_HPP
