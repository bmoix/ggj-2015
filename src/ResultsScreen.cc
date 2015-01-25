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

    GameData* gd = getContext().mGameData;
    assert(gd != NULL);
    ++gd->mRoundsPassed;
    std::string player1Score = "Raccoon\n" + std::to_string(gd->mPointsP1) + " points";
    std::string player2Score = "Fox\n" + std::to_string(gd->mPointsP2) + " points";
    bool finished = (gd->mRoundsPassed >= gd->mNumRounds);

    std::string pressEnter = "Press Enter for next round";
    if (finished) pressEnter = "Press Enter to return to menu";

    // Prepara el text
    std::unique_ptr<AnimatedTextNode> textNode(new AnimatedTextNode(font, pressEnter));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->centerText();
    mText->setInitPosition(sf::Vector2f(1920.0/2.0f, 8.0f*1080.0/9.0f));
    mText->setFinalPosition(sf::Vector2f(1920.0/2.0f, 7.0f*1080.0/8.0f));
    mText->setInitSize(100.0f);
    mText->setFinalSize(120.0f);
    mText->setDuration(1.0f);
    mText->setInitColor(sf::Color::White);
    mText->setFinalColor(sf::Color::White);
    mText->setInitRotation(4.0f);
    mText->setFinalRotation(-2.0f);
    mText->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));

    // Players' texts
    textNode = std::unique_ptr<AnimatedTextNode> (new AnimatedTextNode(font, player1Score));
    //std::unique_ptr<AnimatedTextNode> player1Text(new AnimatedTextNode(font, player1Score));
    mTextNodes[0] = textNode.get();
    mTextNodes[0]->centerText();
    mTextNodes[0]->setInitPosition(sf::Vector2f(1920.0/7.0f, 0.0f));
    mTextNodes[0]->setFinalPosition(sf::Vector2f(1920.0/6.0f, 1080.0f/8.0f));
    mTextNodes[0]->setInitSize(60.0f);
    mTextNodes[0]->setFinalSize(100.0f);
    mTextNodes[0]->setDuration(1.0f);
    mTextNodes[0]->setInitColor(sf::Color::Black);
    mTextNodes[0]->setFinalColor(sf::Color::Red);
    mTextNodes[0]->setInitRotation(-4.0f);
    mTextNodes[0]->setFinalRotation(0.0f);
    mTextNodes[0]->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));

    textNode = std::unique_ptr<AnimatedTextNode> (new AnimatedTextNode(font, player2Score));
    mTextNodes[1] = textNode.get();
    mTextNodes[1]->centerText();
    mTextNodes[1]->setInitPosition(sf::Vector2f(6.0f*1920.0/7.0f, 0.0f));
    mTextNodes[1]->setFinalPosition(sf::Vector2f(5.0f*1920.0/6.0f, 1080.0f/8.0f));
    mTextNodes[1]->setInitSize(60.0f);
    mTextNodes[1]->setFinalSize(100.0f);
    mTextNodes[1]->setDuration(1.0f);
    mTextNodes[1]->setInitColor(sf::Color::Black);
    mTextNodes[1]->setFinalColor(sf::Color::Blue);
    mTextNodes[1]->setInitRotation(2.0f);
    mTextNodes[1]->setFinalRotation(0.0f);
    mTextNodes[1]->initAnimation();
    mSceneLayers[Text]->attachChild(std::move(textNode));
    
    auto gameData = getContext().mGameData;
    if (gameData->mRoundsPassed >= gameData->mNumRounds) {
        context.mMusic->play(Music::WinningTheme);
    }
    else {
        context.mMusic->play(Music::FinishTheme);
    }
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
            getContext().mMusic->stop();
            
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

