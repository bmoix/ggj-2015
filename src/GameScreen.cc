/*********************************GameScreen.cc***********************************/
#include "GameScreen.h"
#include "AnimationNode.h"
#include "Collision.h"

GameScreen::GameScreen(StatesStack& stack, Context& context)
: State(stack, context)
, mJumpVel(2000.0f)
, mMovVel(1000.0f)
, mWalls(2) 
, mPlayer(nullptr)
, mGround(nullptr)
, mCursor(nullptr)
, mTrapButtons(4)
, mTrapsAvailable(3)
, mTextTraps(4)
, mPlatforms(2, nullptr)
, mCountdown() {
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
    sf::Font& font = getContext().mFonts->get(Fonts::Gomo);
    Textures::ID backgroundTexture = Textures::GameBackground1;
    Textures::ID playerTexture = Textures::Player1;
    std::string animationFile = "res/anim/player1.anim";
    if (getContext().mGameData->mSurvivingPlayer == 1) {
        animationFile = "res/anim/player2.anim";
        backgroundTexture = Textures::GameBackground2;
        playerTexture = Textures::Player2;
    }
    sf::Texture& backTexture = getContext().mTextures->get(backgroundTexture);
    sf::Texture& animationTexture = getContext().mTextures->get(playerTexture);
    sf::Texture& wallTexture = getContext().mTextures->get(Textures::Red);
    sf::Texture& groundTexture = getContext().mTextures->get(Textures::Blue);
    sf::Texture& platform1Texture = getContext().mTextures->get(Textures::Platform1);
    sf::Texture& platformWoodTexture = getContext().mTextures->get(Textures::PlatformWood);
    sf::Texture& platformStoneTexture = getContext().mTextures->get(Textures::PlatformStone);
    sf::Texture& cursorTexture = getContext().mTextures->get(Textures::Cursor);
    std::vector<sf::Texture*> iconTexture(4, nullptr);
    iconTexture[0] = &getContext().mTextures->get(Textures::IconBox);
    iconTexture[1] = &getContext().mTextures->get(Textures::IconBall);
    iconTexture[2] = &getContext().mTextures->get(Textures::IconSpikes);
    iconTexture[3] = &getContext().mTextures->get(Textures::IconSwitch);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
    backgroundSprite->setPosition(sf::Vector2f(1920.0/2.0f, 1080.0/2.0f));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    // Add the player to the scene
    std::unique_ptr<Player> player(new Player(animationTexture, animationFile));
    mPlayer = player.get();
    mPlayer->setPosition(500, 500);
    mPlayer->setSize(sf::Vector2u(150, 163));
    mPlayer->createBody(mWorld, true, bbscalex_p1, bbscaley_p1);
    mSceneLayers[Players]->attachChild(std::move(player));

    // Add walls
    std::unique_ptr<InvisibleNode> wallLeft(new InvisibleNode(wallTexture));
    mWalls[0] = wallLeft.get();
    mWalls[0]->setPosition(25.f, 515.f);
    mWalls[0]->setSize(sf::Vector2u(50, 1030));
    mWalls[0]->createBody(mWorld, false,0.8,1.0);
    mSceneLayers[World]->attachChild(std::move(wallLeft));

    std::unique_ptr<InvisibleNode> wallRight(new InvisibleNode(wallTexture));
    mWalls[1] = wallRight.get();
    mWalls[1]->setPosition(1895.f, 515.f);
    mWalls[1]->setSize(sf::Vector2u(50, 1030));
    mWalls[1]->createBody(mWorld, false,0.8,1.0);
    mSceneLayers[World]->attachChild(std::move(wallRight));

    std::unique_ptr<InvisibleNode> ground(new InvisibleNode(groundTexture));
    mGround = ground.get();
    mGround->setPosition(960.f, 1055.f);
    mGround->setSize(sf::Vector2u(1920, 50));
    mGround->createBody(mWorld, false,1.0,0.7);
    mSceneLayers[World]->attachChild(std::move(ground));

    // grass -> 0.6 0.58
    // wood -> 0.8 0.7
    // stone -> 0.8 0.7

    std::unique_ptr<PlatformNode> platform(new PlatformNode(platformWoodTexture));
    mPlatforms[0] = platform.get();
    mPlatforms[0]->setPosition(600.f, 700.f);
    mPlatforms[0]->setSize(sf::Vector2u(250, 75));
    mPlatforms[0]->createBody(mWorld, false, 0.8, 0.7);
    mSceneLayers[World]->attachChild(std::move(platform));

    std::unique_ptr<PlatformNode> platform2(new PlatformNode(platformStoneTexture));
    mPlatforms[1] = platform2.get();
    mPlatforms[1]->setPosition(1200.f, 500.f);
    mPlatforms[1]->setSize(sf::Vector2u(250, 75));
    mPlatforms[1]->createBody(mWorld, false, 0.8, 0.7);
    mPlatforms[1]->changeVisibility();
    mSceneLayers[World]->attachChild(std::move(platform2));

    
    mTrapsAvailable[0] = 10;
    mTrapsAvailable[1] = 5;
    mTrapsAvailable[2] = 1;
    // Add trap buttons
    for (int i = 0; i < (int)mTrapButtons.size(); ++i) {
        std::unique_ptr<SpriteNode> trapButton(new SpriteNode(*iconTexture[i]));
        mTrapButtons[i] = trapButton.get();
        mTrapButtons[i]->setPosition(500.f + i*150.f, 80.f);
        mTrapButtons[i]->setSize(sf::Vector2u(150, 120));
        mSceneLayers[Text]->attachChild(std::move(trapButton));

        if (i < 3) {
            std::unique_ptr<TextNode> textTrap(new TextNode(font, std::to_string(mTrapsAvailable[i])));
            mTextTraps[i] = textTrap.get();
            mTextTraps[i]->setPosition(520.f + i*150.f, 90.f);
            mTextTraps[i]->setCharacterSize(50);
            mTextTraps[i]->setColor(sf::Color::Red);
            mSceneLayers[Text]->attachChild(std::move(textTrap));
        }
    }

    // Prepara el text of timer
    std::unique_ptr<TextNode> textNode(new TextNode(font, "30:00"));
    mText = textNode.get(); // Guarda una referència al TextNode
    mText->setPosition(100, 0);
    mText->setCharacterSize(120);
    mText->setColor(sf::Color::Red);
    mSceneLayers[Text]->attachChild(std::move(textNode));

    // Start timer
    mCountdown.restart();

    std::unique_ptr<SpriteNode> cursorSprite(new SpriteNode(cursorTexture));
    mCursor = cursorSprite.get();
    cursorSprite->setSize(sf::Vector2u(100, 100));
    cursorSprite->setPosition(sf::Vector2f(500.f, 500.f));
    mSceneLayers[Text]->attachChild(std::move(cursorSprite));

    getContext().mMusic->play(Music::GameTheme);
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

    sf::Time elapsed = mCountdown.getElapsedTime();
    std::stringstream ss;
    int sec = elapsed.asMilliseconds();
    ss << std::setfill('0') << std::setw(2) << (29-sec/1000) << "." 
        << std::setfill('0') << std::setw(2) << (1000-sec%1000)/10;
    std::string s;
    ss >> s;
    mText->setString(s);
    if (sec > 20000) {
        mText->setPosition(50, 0);
        mText->setCharacterSize(150);
    }
    if (sec > 25000) {
        mText->setPosition(10, 0);
        mText->setCharacterSize(180);
    }
    if (sec > 30000) {
        requestStackPop();
        requestStackPush(States::Countdown);
        if (getContext().mGameData->mSurvivingPlayer) {
            ++getContext().mGameData->mPointsP2;
        }
        else {
            ++getContext().mGameData->mPointsP1;
        }
        getContext().mGameData->mSurvivingPlayer = 1-getContext().mGameData->mSurvivingPlayer;
    }
    else if (mPlayer->isDead()) {
        requestStackPop();
        requestStackPush(States::Countdown);
        if (getContext().mGameData->mSurvivingPlayer) {
            ++getContext().mGameData->mPointsP1;
        }
        else {
            ++getContext().mGameData->mPointsP2;
        }
        getContext().mGameData->mSurvivingPlayer = 1-getContext().mGameData->mSurvivingPlayer;
    }
    mSceneGraph.update(dt);
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (!mPlayer->isDead()) {
            if (event.key.code == sf::Keyboard::W) {
                if (mPlayer->canJump()) {
                    mPlayer->jump(-mJumpVel);
                }
            }
        }
        if (event.key.code == sf::Keyboard::Num7) {
            if (mTrapsAvailable[Traps::Boxes]) {
                addTrap(Traps::Boxes, mCursor->getWorldPosition());
                mTextTraps[Traps::Boxes]->setString(
                    std::to_string(--mTrapsAvailable[Traps::Boxes])
                );
            }
        }
        if (event.key.code == sf::Keyboard::Num8) {
            if (mTrapsAvailable[Traps::SpikesBall]) {
                addTrap(Traps::SpikesBall, mCursor->getWorldPosition());
                mTextTraps[Traps::SpikesBall]->setString(
                    std::to_string(--mTrapsAvailable[Traps::SpikesBall])
                );
            }

        }
        if (event.key.code == sf::Keyboard::Num9) {
            if (mTrapsAvailable[Traps::Spikes]) {
                addTrap(Traps::Spikes, mCursor->getWorldPosition());
                mTextTraps[Traps::Spikes]->setString(
                    std::to_string(--mTrapsAvailable[Traps::Spikes])
                );
            }

        }
        if (event.key.code == sf::Keyboard::Num0) {
            for (auto pltf : mPlatforms) {
                pltf->changeVisibility();
            }
        }
    }
    return true;
}

void GameScreen::handleRealtimeInput(){
    if (!mPlayer->isDead()) {
        float speed = 20.f;
        sf::Vector2f multivel(0.0,0.0);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            mPlayer->setLookingRight(false);
            mPlayer->setVel(-mMovVel,0.0f);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            mPlayer->setLookingRight(true);
            mPlayer->setVel(mMovVel,0.0f);
        }
        else {
            mPlayer->scaleVel(0.0f, 1.0f);
        }


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            multivel.y-=speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            multivel.y+=speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            multivel.x-=speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            multivel.x+=speed;
        }
        sf::Vector2f pos = mCursor->getWorldPosition();
        if (pos.x + multivel.x > 1820 or pos.x + multivel.x < 100) {
            mCursor->setPosition(pos);
        } else if (pos.y + multivel.y > 980 or pos.y + multivel.y < 100) {
            mCursor->setPosition(pos);
        }
        else {
            mCursor->move(multivel);    
        }
        
        
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
                mTraps[mTraps.size()-1]->createBody(mWorld, true, 0.9, 0.9, 1);
                mSceneLayers[Traps]->attachChild(std::move(trap));
            }
            break;
        }
        case 1: // Spikes ball
        {
            float mVel = 50.0f;
            sf::Texture& trapTexture = getContext().mTextures->get(Textures::SpikesBall);
            std::unique_ptr<SpriteNode> trap(new SpriteNode(trapTexture, CollisionType::Spikes));
            mTraps.push_back(trap.get());
            mTraps[mTraps.size()-1]->setPosition(pos);
            mTraps[mTraps.size()-1]->setSize(sf::Vector2u(85, 85));
            std::srand (std::time(NULL));
            (mTraps[mTraps.size()-1]->createSphericBody(mWorld, true, 0.9, 1))
                ->SetLinearVelocity(b2Vec2(-mVel/2.0f+float(std::rand())/float(RAND_MAX)*mVel, -mVel/2.0f+float(std::rand())/float(RAND_MAX)*mVel));
            
            mSceneLayers[Traps]->attachChild(std::move(trap));
            break;
        }
        case 2: // Spikes
        {
            sf::Texture& trapTexture = getContext().mTextures->get(Textures::Spikes);
            std::unique_ptr<SpriteNode> spikes(new SpriteNode(trapTexture, CollisionType::Spikes));
            spikes->setPosition(pos);
            spikes->setSize(sf::Vector2u(300, 50));
            spikes->createBody(mWorld, false, 0.8, 0.9, 1);
            mTraps.push_back(spikes.get());
            mSceneLayers[Traps]->attachChild(std::move(spikes));
        }
            break;
        case 3: // Platform switcher
            break;
    }
}
