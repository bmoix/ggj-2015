/*********************************GameScreen.cc***********************************/
#include "GameScreen.h"
#include "AnimationNode.h"

GameScreen::GameScreen(StatesStack& stack, Context context)
: State(stack, context)
, mJumpVel(1200.0f)
, mMovVel(300.0f)
, mWalls(2) 
, mGround() {
    // CREACIÓ ESCENA
    // Inicialitza les dues capes
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    // Prepara el fons de pantalla i la font
    sf::Font& font = getContext().mFonts->get(Fonts::Sansation);
    sf::Texture& backTexture = getContext().mTextures->get(Textures::GameBackground);
    sf::Texture& playerTexture = getContext().mTextures->get(Textures::Player1);
    sf::Texture& wallTexture = getContext().mTextures->get(Textures::Red);
    sf::Texture& groundTexture = getContext().mTextures->get(Textures::Blue);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    //centrar la pantalla i escalar la imatge
    backgroundSprite->setPosition(sf::Vector2f(0.0f, 0.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Add the player to the scene
    std::unique_ptr<Player> player(new Player(playerTexture));
    mPlayer = player.get();
    mPlayer->setPosition(0.0f,500-256); // HARD
    mSceneLayers[Players]->attachChild(std::move(player));

    // Add walls
    std::unique_ptr<SpriteNode> wallLeft(new SpriteNode(wallTexture));
    mWalls[0] = wallLeft.get();
    mWalls[0]->setPosition(0.f, 0.f);
    mWalls[0]->setSize(sf::Vector2u(50, 1030));
    mSceneLayers[World]->attachChild(std::move(wallLeft));

    std::unique_ptr<SpriteNode> wallRight(new SpriteNode(wallTexture));
    mWalls[0] = wallRight.get();
    mWalls[0]->setPosition(1870.f, 0.f);
    mWalls[0]->setSize(sf::Vector2u(50, 1030));
    mSceneLayers[World]->attachChild(std::move(wallRight));

    std::unique_ptr<SpriteNode> ground(new SpriteNode(groundTexture));
    mGround = ground.get();
    mGround->setPosition(0.f, 1030.f);
    mGround->setSize(sf::Vector2u(1920, 50));
    mSceneLayers[World]->attachChild(std::move(ground));


    // Prepara el text
    std::unique_ptr<TextNode> textNode(new TextNode(font, "WOLOLO"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setPosition(100 , 0);
    mText->setScale(sf::Vector2f(2,2));
    mSceneLayers[Text]->attachChild(std::move(textNode));

  // Animationtest
    sf::Texture& animationTexture = getContext().mTextures->get(Textures::PlayerAnimation);
    std::unique_ptr<AnimationNode> animation(
        new AnimationNode(animationTexture, "res/anim/player.anim")
    );
    animation->setPosition(500, 500);
    animation->setSize(sf::Vector2u(822, 846));
    mSceneLayers[Players]->attachChild(std::move(animation));

}

void GameScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool GameScreen::update(sf::Time dt) {
    handleRealtimeInput();
    mSceneGraph.update(dt);
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            //if (!mPlayer->isJumping()) {
                mPlayer->isJumping(true);
                mPlayer->setVel(mPlayer->getVel().x,-mJumpVel);
            //}
        }
        else if (event.key.code == sf::Keyboard::A) {
            mPlayer->setVel(-mMovVel,mPlayer->getVel().y);
        }
        else if (event.key.code == sf::Keyboard::D) {
            mPlayer->setVel(mMovVel,mPlayer->getVel().y);
        }
    } 
    return true;
}

void GameScreen::handleRealtimeInput(){
}
