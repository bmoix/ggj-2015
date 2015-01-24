/*********************************GameScreen.cc***********************************/
#include "GameScreen.h"
#include "AnimationNode.h"
#include "Collision.h"

GameScreen::GameScreen(StatesStack& stack, Context context)
: State(stack, context)
, mJumpVel(2000.0f)
, mMovVel(1000.0f)
, mWalls(2) 
, mGround()
, mTrapButtons(5) {
    // CREACIÓ ESCENA
    // Create box2D world;
    const b2Vec2 gravity(0, 30.0f);
    mWorld = new b2World(gravity);
    mWorld->SetContactListener(new Collision());

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
    sf::Texture& platform1Texture = getContext().mTextures->get(Textures::Platform1);
    sf::Texture& platformWoodTexture = getContext().mTextures->get(Textures::PlatformWood);
    sf::Texture& platformStoneTexture = getContext().mTextures->get(Textures::PlatformStone);
    sf::Texture& animationTexture = getContext().mTextures->get(Textures::PlayerAnimation);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    //centrar la pantalla i escalar la imatge
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.0f, 1080.0/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Add the player to the scene
    std::unique_ptr<Player> player(new Player(animationTexture, "res/anim/player1.anim"));
    mPlayer = player.get();
    mPlayer->setPosition(500, 500);
    mPlayer->setSize(sf::Vector2u(150, 163));
    mPlayer->createBody(mWorld, true, bbscalex_p1, bbscaley_p1);
    mSceneLayers[Players]->attachChild(std::move(player));

    // Add walls
    std::unique_ptr<SpriteNode> wallLeft(new SpriteNode(wallTexture));
    mWalls[0] = wallLeft.get();
    mWalls[0]->setPosition(25.f, 515.f);
    mWalls[0]->setSize(sf::Vector2u(50, 1030));
    mWalls[0]->createBody(mWorld, false);
    mWalls[0]->mType = 1;
    mSceneLayers[World]->attachChild(std::move(wallLeft));

    std::unique_ptr<SpriteNode> wallRight(new SpriteNode(wallTexture));
    mWalls[1] = wallRight.get();
    mWalls[1]->setPosition(1895.f, 515.f);
    mWalls[1]->setSize(sf::Vector2u(50, 1030));
    mWalls[1]->createBody(mWorld, false);
    mWalls[1]->mType = 2;
    mSceneLayers[World]->attachChild(std::move(wallRight));

    std::unique_ptr<SpriteNode> ground(new SpriteNode(groundTexture));
    mGround = ground.get();
    mGround->setPosition(960.f, 1055.f);
    mGround->setSize(sf::Vector2u(1920, 50));
    mGround->createBody(mWorld, false);
    mSceneLayers[World]->attachChild(std::move(ground));

    // grass -> 0.6 0.58
    // wood -> 0.8 0.7
    // stone -> 0.8 0.7

    std::unique_ptr<SpriteNode> platform(new SpriteNode(platformWoodTexture));
    platform->setPosition(600.f, 700.f);
    platform->setSize(sf::Vector2u(250, 75));
    platform->createBody(mWorld, false, 0.8, 0.7);
    mSceneLayers[World]->attachChild(std::move(platform));

    std::unique_ptr<SpriteNode> platform2(new SpriteNode(platformStoneTexture));
    platform2->setPosition(1200.f, 500.f);
    platform2->setSize(sf::Vector2u(250, 75));
    platform2->createBody(mWorld, false, 0.8, 0.7);
    mSceneLayers[World]->attachChild(std::move(platform2));

    // Prepara el text
    std::unique_ptr<TextNode> textNode(new TextNode(font, "WOLOLO"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setPosition(100 , 0);
    mText->setScale(sf::Vector2f(2,2));
    mSceneLayers[Text]->attachChild(std::move(textNode));

    // Add trap buttons
    for (int i = 0; i < (int)mTrapButtons.size(); ++i) {
        std::unique_ptr<SpriteNode> trapButton(new SpriteNode(wallTexture));
        mTrapButtons[i] = trapButton.get();
        mTrapButtons[i]->setPosition(500.f + i*100.f, 50.f);
        mTrapButtons[i]->setSize(sf::Vector2u(75, 75));
        mSceneLayers[Text]->attachChild(std::move(trapButton));
    }

}

void GameScreen::draw() {
    getContext().mRTexture->draw(mSceneGraph);
}
bool GameScreen::update(sf::Time dt) {
    handleRealtimeInput();
    //Box2d update
    const unsigned int velocityIterations = 6;
    const unsigned int positionIterations = 2;
    mWorld->Step(dt.asSeconds(), velocityIterations, positionIterations);
    mSceneGraph.update(dt);
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            if (mPlayer->canJump()) {
                mPlayer->jump(-mJumpVel);
                //mPlayer->setVel(mPlayer->getVel().x,-mJumpVel);
            }
        }
        if (event.key.code == sf::Keyboard::Num1) {
            addTrap(0, sf::Vector2f(500, 50));
        }
    }
    return true;
}

void GameScreen::handleRealtimeInput(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        mPlayer->setVel(-mMovVel,0.0f);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        mPlayer->setVel(mMovVel,0.0f);
    }
    else {
        mPlayer->scaleVel(0.0f, 1.0f);
    }

}

void GameScreen::addTrap(int type, sf::Vector2f pos) {
    switch(type) {
        case 0: // Box line
        {
            sf::Texture& trapTexture = getContext().mTextures->get(Textures::TrapBox);
            for (int i = 0; i < 5; ++i) {
                std::unique_ptr<SpriteNode> trap(new SpriteNode(trapTexture));
                mTraps.push_back(trap.get());
                mTraps[mTraps.size()-1]->setPosition(pos);
                mTraps[mTraps.size()-1]->setSize(sf::Vector2u(75, 75));
                mTraps[mTraps.size()-1]->createBody(mWorld, true, 0.9, 0.9);
                mTraps[mTraps.size()-1]->mType = 1;
                mSceneLayers[Traps]->attachChild(std::move(trap));
            }
            break;
        }
        case 1: // Spikes ball
            break;
        case 2: // Spikes
            break;
        case 3: // Platform switcher
            break;
    }
}
