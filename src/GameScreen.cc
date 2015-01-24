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
    // Create box2D world;
    const b2Vec2 gravity(0, 10.0f);
    mWorld = new b2World(gravity);
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
    sf::Texture& animationTexture = getContext().mTextures->get(Textures::PlayerAnimation);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    //centrar la pantalla i escalar la imatge
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.0f, 1080.0/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Add the player to the scene
    std::unique_ptr<Player> player(new Player(animationTexture, "res/anim/player.anim"));
    mPlayer = player.get();
    mPlayer->setPosition(500, 500);
    mPlayer->setSize(sf::Vector2u(411, 423));
    mPlayer->createBody(mWorld, true);
    mSceneLayers[Players]->attachChild(std::move(player));

    // Add walls
    std::unique_ptr<SpriteNode> wallLeft(new SpriteNode(wallTexture));
    mWalls[0] = wallLeft.get();
    mWalls[0]->setPosition(25.f, 515.f);
    mWalls[0]->setSize(sf::Vector2u(50, 1030));
    //mWalls[0]->createBody(mWorld, false);
    mSceneLayers[World]->attachChild(std::move(wallLeft));

    std::unique_ptr<SpriteNode> wallRight(new SpriteNode(wallTexture));
    mWalls[1] = wallRight.get();
    mWalls[1]->setPosition(1895.f, 515.f);
    mWalls[1]->setSize(sf::Vector2u(50, 1030));
    //mWalls[1]->createBody(mWorld, false);
    mSceneLayers[World]->attachChild(std::move(wallRight));

    std::unique_ptr<SpriteNode> ground(new SpriteNode(groundTexture));
    mGround = ground.get();
    mGround->setPosition(960.f, 1055.f);
    mGround->setSize(sf::Vector2u(1920, 50));
    mGround->createBody(mWorld, false);
    mSceneLayers[World]->attachChild(std::move(ground));


    // Prepara el text
    std::unique_ptr<TextNode> textNode(new TextNode(font, "WOLOLO"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setPosition(100 , 0);
    mText->setScale(sf::Vector2f(2,2));
    mSceneLayers[Text]->attachChild(std::move(textNode));

}

void GameScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool GameScreen::update(sf::Time dt) {
    handleRealtimeInput();
    mSceneGraph.update(dt);
    //Box2d update
    
    const unsigned int velocityIterations = 6;
    const unsigned int positionIterations = 2;
    mWorld->Step(dt.asSeconds(), velocityIterations, positionIterations);
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            //if (!mPlayer->isJumping()) {
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
