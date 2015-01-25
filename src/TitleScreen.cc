/*********************************TitleScreen.cc***********************************/
#include "TitleScreen.h"

TitleScreen::TitleScreen(StatesStack& stack, Context& context)
: State(stack, context) {
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    // Prepara el fons de pantalla i la font
    sf::Font& font = getContext().mFonts->get(Fonts::Sansation);
    sf::Texture& backTexture = getContext().mTextures->get(Textures::Title);
    sf::Texture& startTexture = getContext().mTextures->get(Textures::Start);
    sf::Texture& helpTexture = getContext().mTextures->get(Textures::Help);
    sf::Texture& exitTexture = getContext().mTextures->get(Textures::Exit);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.f, 1080.0f/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    std::unique_ptr<SpriteNode> button1(new SpriteNode(startTexture));
    mButtonPlay = button1.get();
    mButtonPlay->setPosition(325.f, 570.f);
    mButtonPlay->setSize(sf::Vector2u(600, 200));
    mSceneLayers[Text]->attachChild(std::move(button1));

    std::unique_ptr<SpriteNode> button2(new SpriteNode(helpTexture));
    mButtonHelp = button2.get();
    mButtonHelp->setPosition(325.f, 770.f);
    mButtonHelp->setSize(sf::Vector2u(600, 200));
    mSceneLayers[Text]->attachChild(std::move(button2));

    std::unique_ptr<SpriteNode> button3(new SpriteNode(exitTexture));
    mButtonExit = button3.get();
    mButtonExit->setPosition(325.f, 970.f);
    mButtonExit->setSize(sf::Vector2u(600, 200));
    mSceneLayers[Text]->attachChild(std::move(button3));

    context.mMusic->play(Music::MenuTheme);
}

void TitleScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool TitleScreen::update(sf::Time dt) {
    return true;
}

bool TitleScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::EventType::MouseButtonPressed 
        && event.mouseButton.button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().mWindow);
        sf::Vector2i newPos = Utils::correctMouse(mousePos, getContext().mScale);
        sf::IntRect newGameBounds = mButtonPlay->getBounds();
        sf::IntRect helpBounds = mButtonHelp->getBounds();
        sf::IntRect exitBounds = mButtonExit->getBounds();
        if (newGameBounds.contains(newPos)) {
            getContext().mMusic->stop();
            getContext().mSound->play(SoundEffect::Gong);
            requestStackPop();
            requestStackPush(States::Countdown);
        }
        else if (helpBounds.contains(newPos)) {
            requestStackPop();
            requestStackPush(States::Help);
        }
        else if (exitBounds.contains(newPos)) {
            requestStackPop();
        }
        else {
            std::cout << "kek" << std::endl;
        } 
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) {
            requestStackPop();
            requestStackPush(States::Countdown);
        }
    }
    return true;
}

void TitleScreen::click(mouseButtons mouseButton, sf::Vector2f mouseClick) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //sf::Vector2i newMouse = Utils::correctMouse(mouseClick, mouseClick);
    }
}
