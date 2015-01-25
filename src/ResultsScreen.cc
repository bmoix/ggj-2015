/*********************************ResultsScreen.cc***********************************/
#include "ResultsScreen.h"

ResultsScreen::ResultsScreen(StatesStack& stack, Context& context) : 
    State(stack, context) {
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    // Prepara el fons de pantalla i la font
    sf::Font& font = getContext().mFonts->get(Fonts::Gomo);

    Textures::ID winningTextureID = Textures::RaccoonWins;
    if (getContext().mGameData->mWinningPlayer == 1) {
        winningTextureID = Textures::FoxWins;
    }

    sf::Texture& winningTexture = getContext().mTextures->get(winningTextureID);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> winningSprite(new SpriteNode(winningTexture));
    //centrar la pantalla i escalar la imatge
    winningSprite->setPosition(sf::Vector2f(1920.0/2.f, 1080.0f/2.0f));
    mSceneLayers[Background]->attachChild(std::move(winningSprite));

    // Prepara el text
    std::unique_ptr<AnimatedTextNode> textNode(new AnimatedTextNode(font, "Scores"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->centerText();
    mText->setInitPosition(sf::Vector2f(1920.0/2.0f, 50.0f));
    mText->setFinalPosition(sf::Vector2f(1920.0/2.0f, 60.0f));
    mText->setInitSize(100.0f);
    mText->setFinalSize(120.0f);
    mText->setDuration(5.0f);
    mText->setInitColor(sf::Color::Black);
    mText->setFinalColor(sf::Color::Red);
    mText->setInitRotation(-10.0f);
    mText->setFinalRotation(10.0f);
    mText->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));

    GameData* gd = getContext().mGameData;
    assert(gd != NULL);
    ++gd->mRoundsPassed;
    std::string player1Score = "Raccoon\n" + std::to_string(gd->mPointsP1) + " points";
    std::string player2Score = "Fox\n" + std::to_string(gd->mPointsP2) + " points";

    // Players' texts
    textNode = std::unique_ptr<AnimatedTextNode> (new AnimatedTextNode(font, player1Score));
    //std::unique_ptr<AnimatedTextNode> player1Text(new AnimatedTextNode(font, player1Score));
    mTextNodes[0] = textNode.get();
    mTextNodes[0]->centerText();
    mTextNodes[0]->setInitPosition(sf::Vector2f(1920.0/7.0f, 1080.0f/8.0f));
    mTextNodes[0]->setFinalPosition(sf::Vector2f(1920.0/6.0f, 1080.0f/6.0f));
    mTextNodes[0]->setInitSize(60.0f);
    mTextNodes[0]->setFinalSize(100.0f);
    mTextNodes[0]->setDuration(3.0f);
    mTextNodes[0]->setInitColor(sf::Color::White);
    mTextNodes[0]->setFinalColor(sf::Color::White);
    mTextNodes[0]->setInitRotation(-4.0f);
    mTextNodes[0]->setFinalRotation(4.0f);
    mTextNodes[0]->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));

    textNode = std::unique_ptr<AnimatedTextNode> (new AnimatedTextNode(font, player2Score));
    mTextNodes[1] = textNode.get();
    mTextNodes[1]->centerText();
    mTextNodes[1]->setInitPosition(sf::Vector2f(6.0f*1920.0/7.0f, 1080.0f/8.0f));
    mTextNodes[1]->setFinalPosition(sf::Vector2f(5.0f*1920.0/6.0f, 1080.0f/6.0f));
    mTextNodes[1]->setInitSize(60.0f);
    mTextNodes[1]->setFinalSize(100.0f);
    mTextNodes[1]->setDuration(3.0f);
    mTextNodes[1]->setInitColor(sf::Color::White);
    mTextNodes[1]->setFinalColor(sf::Color::White);
    mTextNodes[1]->setInitRotation(2.0f);
    mTextNodes[1]->setFinalRotation(-5.0f);
    mTextNodes[1]->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));
}

void ResultsScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool ResultsScreen::update(sf::Time dt) {
    mSceneGraph.update(dt);
    return false;
}

bool ResultsScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) {
            requestStackPop();
            requestStackPop();

            GameData* gd = getContext().mGameData;
            assert(gd != NULL);
            // Finished game, reset number of rounds, score and return to title
            if (gd->mRoundsPassed >= gd->mNumRounds) {
                gd->mRoundsPassed = 0;
                gd->mPointsP1 = gd->mPointsP2 = 0;
                requestStackPush(States::Title);
            }
            else {
                requestStackPush(States::Countdown);
            }
        }
    }
    return false;
}

