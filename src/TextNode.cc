#include "TextNode.h"

TextNode::TextNode(const sf::Font& font, const std::string& text) {
	mText.setFont(font);
	mText.setColor(sf::Color::Black);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mText, states);
}

void TextNode::setString(const std::string& text) {
	mText.setString(sf::String(Utils::utf8_to_utf16(text)));
    //Utils::centerOrigin(mText);
}

void TextNode::setCharacterSize(unsigned int size) {
	mText.setCharacterSize(size);
}

void TextNode::setColor(const sf::Color& color) {
	mText.setColor(color);
}

void TextNode::centerText() {
    sf::Vector2f initPos = mText.getPosition();
    sf::FloatRect textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    mText.setPosition(initPos);
}
