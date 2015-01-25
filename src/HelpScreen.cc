/*********************************HelpScreen.cc***********************************/
#include "HelpScreen.h"

HelpScreen::HelpScreen(StatesStack& stack, Context& context)
: State(stack, context) {
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    // Prepara el fons de pantalla i la font
    sf::Font& font = getContext().mFonts->get(Fonts::Gomo);
    sf::Texture& backTexture = getContext().mTextures->get(Textures::HelpBackground);
    sf::Texture& returnTexture = getContext().mTextures->get(Textures::Return);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.f, 1080.0f/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    std::unique_ptr<SpriteNode> buttonBack(new SpriteNode(returnTexture));
    mButtonBack = buttonBack.get();
    mButtonBack->setPosition(150, 150);
    mButtonBack->setSize(sf::Vector2u(100, 100));
    mSceneLayers[Text]->attachChild(std::move(buttonBack));

    std::unique_ptr<TextNode> textHelp(new TextNode(font, "SURVIVAL MODE"));
    mText = textHelp.get();
    mText->setPosition(1100.f, 260.f);
    mText->setCharacterSize(100);
    mText->setColor(sf::Color::Black);
    mSceneLayers[Text]->attachChild(std::move(textHelp));

    std::unique_ptr<TextNode> textHelp2(new TextNode(font, "KILLER MODE"));
    mText2 = textHelp2.get();
    mText2->setPosition(1175.f, 760.f);
    mText2->setCharacterSize(100);
    mText2->setColor(sf::Color::Black);
    mSceneLayers[Text]->attachChild(std::move(textHelp2));

    std::unique_ptr<TextNode> textWat(new TextNode(font, "WAT DO"));
    mTextWat = textWat.get();
    mTextWat->setPosition(1250.f, 430.f);
    mTextWat->setCharacterSize(150);
    mTextWat->setRotation(15);
    mTextWat->setColor(sf::Color::Red);
    mSceneLayers[Text]->attachChild(std::move(textWat));
}

void HelpScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool HelpScreen::update(sf::Time dt) {
    return true;
}

bool HelpScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::EventType::MouseButtonPressed 
        && event.mouseButton.button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().mWindow);
        sf::Vector2i newPos = Utils::correctMouse(mousePos, getContext().mScale);
        sf::IntRect buttonBounds = mButtonBack->getBounds();
        if (buttonBounds.contains(newPos)) {
            requestStackPop();
            requestStackPush(States::Title);
        }
    }
    return true;
}

void HelpScreen::click(mouseButtons mouseButton, sf::Vector2f mouseClick) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //sf::Vector2i newMouse = Utils::correctMouse(mouseClick, mouseClick);
    }
}
