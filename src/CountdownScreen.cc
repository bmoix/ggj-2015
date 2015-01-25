/*********************************CountdownScreen.cc***********************************/
#include "CountdownScreen.h"
#include <memory>

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

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    //centrar la pantalla i escalar la imatge
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.f, 1080.0f/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Prepara el text
    std::unique_ptr<AnimatedTextNode> textNode(new AnimatedTextNode(font, "3"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setString("3");
    mText->centerText();
    mText->setInitPosition(sf::Vector2f(1920.0/2.0f, 1080.0f/1.2f));
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

    GameData* gd = getContext().mGameData;
    assert(gd != NULL);
    std::string player1Mission = "Raccoon ";
    std::string player2Mission = "Fox ";
    if (gd->mSurvivingPlayer == 0) {
        player1Mission += "Survives";
        player2Mission += "Attacks";
    }
    else {
        player1Mission += "Attacks";
        player2Mission += "Survives";
    }
    // Players' texts
    textNode = std::unique_ptr<AnimatedTextNode> (new AnimatedTextNode(font, player1Mission));
    //std::unique_ptr<AnimatedTextNode> player1Text(new AnimatedTextNode(font, player1Mission));
    mTextNodes[0] = textNode.get();
    mTextNodes[0]->centerText();
    mTextNodes[0]->setInitPosition(sf::Vector2f(1920.0/4.0f, 1080.0f/8.0f));
    mTextNodes[0]->setFinalPosition(sf::Vector2f(1920.0/4.0f, 1080.0f/6.0f));
    mTextNodes[0]->setInitSize(60.0f);
    mTextNodes[0]->setFinalSize(100.0f);
    mTextNodes[0]->setDuration(3.0f);
    mTextNodes[0]->setInitColor(sf::Color::Red);
    mTextNodes[0]->setFinalColor(sf::Color::Black);
    mTextNodes[0]->setInitRotation(-4.0f);
    mTextNodes[0]->setFinalRotation(4.0f);
    mTextNodes[0]->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));

    textNode = std::unique_ptr<AnimatedTextNode> (new AnimatedTextNode(font, player2Mission));
    mTextNodes[1] = textNode.get();
    mTextNodes[1]->centerText();
    mTextNodes[1]->setInitPosition(sf::Vector2f(3.0f*1920.0/4.0f, 1080.0f/8.0f));
    mTextNodes[1]->setFinalPosition(sf::Vector2f(3.0f*1920.0/4.0f, 1080.0f/6.0f));
    mTextNodes[1]->setInitSize(60.0f);
    mTextNodes[1]->setFinalSize(100.0f);
    mTextNodes[1]->setDuration(3.0f);
    mTextNodes[1]->setInitColor(sf::Color::Blue);
    mTextNodes[1]->setFinalColor(sf::Color::Black);
    mTextNodes[1]->setInitRotation(2.0f);
    mTextNodes[1]->setFinalRotation(-5.0f);
    mTextNodes[1]->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));

    getContext().mMusic->play(Music::StartTheme);
}

void CountdownScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool CountdownScreen::update(sf::Time dt) {
    mSceneGraph.update(dt);
    float timeLeft = mWaitingTime - mClock.getElapsedTime().asSeconds();
    if (timeLeft < 0.0f) {
        getContext().mMusic->stop();
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

