/*********************************TitleScreen.cc***********************************/
#include "TitleScreen.h"

TitleScreen::TitleScreen(StatesStack& stack, Context context)
: State(stack, context) {
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    // Prepara el fons de pantalla i la font
    sf::Font& font = getContext().mFonts->get(Fonts::Sansation);
    sf::Texture& backTexture = getContext().mTextures->get(Textures::GameBackground);
    sf::Texture& button1Texture = getContext().mTextures->get(Textures::Red);
    sf::Texture& button2Texture = getContext().mTextures->get(Textures::Blue);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    //centrar la pantalla i escalar la imatge
    backgroundSprite->setPosition(sf::Vector2f(0.0f, 0.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    std::unique_ptr<SpriteNode> button1(new SpriteNode(button1Texture));
    mButtonPlay = button1.get();
    mButtonPlay->setPosition(1920.f/2.f, 500.f);
    mButtonPlay->setSize(sf::Vector2u(600, 100));
    mSceneLayers[Text]->attachChild(std::move(button1));

    std::unique_ptr<SpriteNode> button2(new SpriteNode(button2Texture));
    mButtonHelp = button2.get();
    mButtonHelp->setPosition(1920.f/2.f, 650.f);
    mButtonHelp->setSize(sf::Vector2u(600, 100));
    mSceneLayers[Text]->attachChild(std::move(button2));

    std::unique_ptr<SpriteNode> button3(new SpriteNode(button2Texture));
    mButtonExit = button3.get();
    mButtonExit->setPosition(1920.f/2.f, 800.f);
    mButtonExit->setSize(sf::Vector2u(600, 100));
    mSceneLayers[Text]->attachChild(std::move(button3));

    // Prepara el text
    std::unique_ptr<TextNode> textNode(new TextNode(font, "TOP KEK"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setPosition(1920/2, 200);
    mText->setScale(sf::Vector2f(5,5));
    mSceneLayers[Text]->attachChild(std::move(textNode));
}

void TitleScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool TitleScreen::update(sf::Time dt) {
    return true;
}

bool TitleScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::EventType::MouseButtonPressed 
        and event.mouseButton.button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().mWindow);
        sf::Vector2i newPos = Utils::correctMouse(mousePos, getContext().mScale);
        sf::IntRect bounds = mButtonPlay->getBounds();
        cout << bounds.left << " " << bounds.top << " " << bounds.width << " " << bounds.height << endl;
        if (bounds.contains(newPos)) {
            requestStackPop();
            requestStackPush(States::Game);
        }
        else {
            cout << "kek" << endl;
        }
    }
    return true;
}

void TitleScreen::click(mouseButtons mouseButton, sf::Vector2f mouseClick) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //sf::Vector2i newMouse = Utils::correctMouse(mouseClick, mouseClick);
    }
}
