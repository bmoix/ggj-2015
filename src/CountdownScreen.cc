/*********************************CountdownScreen.cc***********************************/
#include "CountdownScreen.h"

CountdownScreen::CountdownScreen(StatesStack& stack, Context& context) : 
    State(stack, context),
    mWaitingTime(3.0f) {
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    // Prepara el fons de pantalla i la font
    sf::Font& font = getContext().mFonts->get(Fonts::Gomo);
    sf::Texture& backTexture = getContext().mTextures->get(Textures::CountdownBackground);
    //sf::Texture& player1Texture = getContext().mTextures->Get(Textures::FacePlayer1);
    //sf::Texture& player1Texture = getContext().mTextures->Get(Textures::FacePlayer1);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    //centrar la pantalla i escalar la imatge
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.f, 1080.0f/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));


    // Prepara el text
    std::unique_ptr<AnimatedTextNode> textNode(new AnimatedTextNode(font, "3"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setCharacterSize(500.0f);
    mText->setString("3");
    mText->centerText();
    mText->setInitPosition(sf::Vector2f(1920.0/2.0f, 1080.0f/2.0f));
    mText->setFinalPosition(sf::Vector2f(1920.0/2.0f, 1080.0f/2.0f));
    mText->setInitSize(100.0f);
    mText->setFinalSize(500.0f);
    mText->setDuration(3.0f);
    mText->setInitColor(sf::Color::Black);
    mText->setFinalColor(sf::Color::Red);
    mText->setInitRotation(-10.0f);
    mText->setFinalRotation(10.0f);

    mText->initAnimation();
    

    mSceneLayers[Text]->attachChild(std::move(textNode));
}

void CountdownScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool CountdownScreen::update(sf::Time dt) {
    mSceneGraph.update(dt);
    float timeLeft = mWaitingTime - mClock.getElapsedTime().asSeconds();
    if (timeLeft < 0.0f) {
        requestStackPop();
        requestStackPush(States::Game);
    }
    else {
        mText->setString(std::to_string(int(std::ceil(timeLeft))));
        mText->centerText();
    }
    return false;
}

bool CountdownScreen::handleEvent(const sf::Event& event) {
    return false;
}

