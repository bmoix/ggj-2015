/*********************************GameScreen.cc***********************************/
#include "GameScreen.h"
#include "AnimationNode.h"
#include "Collision.h"

GameScreen::GameScreen(StatesStack& stack, Context& context)
: State(stack, context)
, mWalls(2)
, mGround(nullptr)
, mCursor(nullptr)
, mTrapButtons(4)
, mPlatforms(12, nullptr)
, mFixedPlatforms(4, nullptr)
, mPlayer(nullptr)
, mJumpVel(2000.0f)
, mMovVel(1000.0f)
, wololo(false)
, topkek(false)
, mGamepad1(-1)
, mGamepad2(-1)
, mHasGamepads(false)
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
    sf::Texture& platformTexture = getContext().mTextures->get(Textures::Platform1);
    sf::Texture& platformWoodTexture = getContext().mTextures->get(Textures::PlatformWood);
    sf::Texture& platformStoneTexture = getContext().mTextures->get(Textures::PlatformStone);
    sf::Texture& cursorTexture = getContext().mTextures->get(Textures::Cursor);

    std::vector<std::string> iconAnimFilename {
        "res/anim/iconBox.anim",
        "res/anim/iconBall.anim",
        "res/anim/iconSpikes.anim",
        "res/anim/iconSwitch.anim"
    };

    std::vector<sf::Texture*> iconTexture(4, nullptr);
    iconTexture[0] = &getContext().mTextures->get(Textures::IconBox);
    iconTexture[1] = &getContext().mTextures->get(Textures::IconBall);
    iconTexture[2] = &getContext().mTextures->get(Textures::IconSpikes);
    iconTexture[3] = &getContext().mTextures->get(Textures::IconSwitch);
    std::vector<sf::Texture*> buttonTexture(4, nullptr);
    buttonTexture[0] = &getContext().mTextures->get(Textures::AButton);
    buttonTexture[1] = &getContext().mTextures->get(Textures::BButton);
    buttonTexture[2] = &getContext().mTextures->get(Textures::XButton);
    buttonTexture[3] = &getContext().mTextures->get(Textures::YButton);

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
    mWalls[0]->setCollisionType(CollisionType::Wall);
    mSceneLayers[World]->attachChild(std::move(wallLeft));

    std::unique_ptr<InvisibleNode> wallRight(new InvisibleNode(wallTexture));
    mWalls[1] = wallRight.get();
    mWalls[1]->setPosition(1895.f, 515.f);
    mWalls[1]->setSize(sf::Vector2u(50, 1030));
    mWalls[1]->createBody(mWorld, false,0.8,1.0);
    mWalls[1]->setCollisionType(CollisionType::Wall);
    mSceneLayers[World]->attachChild(std::move(wallRight));

    // Ceiling
    std::unique_ptr<InvisibleNode> ceiling(new InvisibleNode(wallTexture));
    ceiling->setPosition(960.f, -50.f);
    // Aparently same size as the ground is not enough to cover same space
    ceiling->setSize(sf::Vector2u(2500, 50));
    ceiling->createBody(mWorld, false,0.8,1.0);
    ceiling->setCollisionType(CollisionType::Wall);
    mSceneLayers[World]->attachChild(std::move(ceiling));

    std::unique_ptr<InvisibleNode> ground(new InvisibleNode(groundTexture));
    mGround = ground.get();
    mGround->setPosition(960.f, 1055.f);
    mGround->setSize(sf::Vector2u(1920, 50));
    mGround->createBody(mWorld, false,1.0,0.7);
    mGround->setCollisionType(CollisionType::Ground);
    mSceneLayers[World]->attachChild(std::move(ground));

    // grass -> 0.6 0.58
    // wood -> 0.8 0.7
    // stone -> 0.8 0.7

    std::vector<sf::Vector2f> fixedPos(4);
    fixedPos[0] = sf::Vector2f(150, 400);
    fixedPos[1] = sf::Vector2f(150, 800);
    fixedPos[2] = sf::Vector2f(1800, 400);
    fixedPos[3] = sf::Vector2f(1800, 800);
    for (int i = 0; i < 4; ++i) {
        sf::Texture* ptrTexture = nullptr;
        if (i%2) ptrTexture = &platformWoodTexture;
        else ptrTexture = &platformStoneTexture;
        std::unique_ptr<PlatformNode> platform(new PlatformNode(*ptrTexture));
        mPlatforms[i] = platform.get();
        mPlatforms[i]->setPosition(600.f + i*200.f, 550.f);
        mPlatforms[i]->setSize(sf::Vector2u(175, 60));
        mPlatforms[i]->createBody(mWorld, false, 0.75, 0.7);
        mPlatforms[i]->setCollisionType(CollisionType::Wall);
        if (i%2) mPlatforms[i]->changeVisibility();
        mSceneLayers[World]->attachChild(std::move(platform));

        std::unique_ptr<PlatformNode> platform2(new PlatformNode(platformTexture));
        mFixedPlatforms[i] = platform2.get();
        mFixedPlatforms[i]->setPosition(fixedPos[i]);
        mFixedPlatforms[i]->setSize(sf::Vector2u(200, 70));
        mFixedPlatforms[i]->createBody(mWorld, false, 0.6, 0.58);
        mFixedPlatforms[i]->setCollisionType(CollisionType::Wall);
        mSceneLayers[World]->attachChild(std::move(platform2));
    }

    std::vector<sf::Vector2f> prisonPos(4);
    prisonPos[0] = sf::Vector2f(450, 100);
    prisonPos[1] = sf::Vector2f(550, 200);
    prisonPos[2] = sf::Vector2f(450, 300);
    prisonPos[3] = sf::Vector2f(350, 200);
    for (int i = 0; i < 4; ++i) {
        std::unique_ptr<PlatformNode> platform(new PlatformNode(platformWoodTexture));
        mPlatforms[4+i] = platform.get();
        mPlatforms[4+i]->setPosition(prisonPos[i]);
        mPlatforms[4+i]->setSize(sf::Vector2u(175, 60));
        //if (i%2) mPlatforms[4+i]->setRotation(90);
        float rotation = 0.0f;
        if (i%2) rotation = atan(1)*2.0f;
        mPlatforms[4+i]->createBody(mWorld, false, 0.8, 0.3, 1.0f, rotation);
        mPlatforms[4+i]->setCollisionType(CollisionType::Wall);
        mPlatforms[4+i]->changeVisibility();
        mSceneLayers[World]->attachChild(std::move(platform));

        std::unique_ptr<PlatformNode> platform2(new PlatformNode(platformStoneTexture));
        mPlatforms[8+i] = platform2.get();
        mPlatforms[8+i]->setPosition(prisonPos[i].x + 1000, prisonPos[i].y);
        mPlatforms[8+i]->setSize(sf::Vector2u(200, 70));
        rotation = 0.0f;
        if (i%2) rotation = atan(1)*2.0f;
        mPlatforms[8+i]->createBody(mWorld, false, 0.8, 0.3, 1.0f, rotation);
        mPlatforms[8+i]->setCollisionType(CollisionType::Wall);
        mSceneLayers[World]->attachChild(std::move(platform2));
    }
    
    // TRAPS
    // Trap cooldown, must be aligned with anim
    mTrapCooldown[Traps::Boxes] = 1.6f;
    mTrapCooldown[Traps::SpikesBall] = 6.0f;
    mTrapCooldown[Traps::Spikes] = 6.0f;
    mTrapCooldown[Traps::Platform] = 0.5f;


    // Add trap buttons
    for (int i = 0; i < (int)mTrapButtons.size(); ++i) {
        std::unique_ptr<AnimationNode> trapButton(new AnimationNode(*iconTexture[i], 
                                                                 iconAnimFilename[i]));
        mTrapButtons[i] = trapButton.get();
        mTrapButtons[i]->setPosition(500.f + i*150.f, 80.f);
        mTrapButtons[i]->setSize(sf::Vector2u(150, 120));
        mSceneLayers[Text]->attachChild(std::move(trapButton));

        std::unique_ptr<SpriteNode> gamepadButton(new SpriteNode(*buttonTexture[i]));
        SpriteNode* sprite = gamepadButton.get();
        sprite->setPosition(500.f + i*150.f, 30.f);
        sprite->setSize(sf::Vector2u(50, 50));
        mSceneLayers[Text]->attachChild(std::move(gamepadButton));

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

    // COMPROVA GAMEPADS
    for (int i = 0; i < 8; ++i) {
        if (sf::Joystick::isConnected(i)) {
            if (mGamepad1 == -1) {
                mGamepad1 = i;
            }
            else {
                mGamepad2 = i;
            }
        }
    }
    mHasGamepads = (mGamepad2 != -1); // Check if there are two gamepads
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
        if (!topkek) {
            for (int i = 0; i < 5; ++i) {
                addTrap(2, sf::Vector2f(250 + 350*i, 1020));
            }
            topkek = true;
        }
        mText->setPosition(50, 0);
        mText->setCharacterSize(150);
    }
    if (sec > 25000) {
        if (!wololo) {
            for (auto pltf : mFixedPlatforms) {
                pltf->changeVisibility();
            }
            wololo = true;
        }
        mText->setPosition(10, 0);
        mText->setCharacterSize(180);
    }
    if (sec > 30000 || mPlayer->isDead()) {
        // I am going to hell for this
        mText->setPosition(sf::Vector2f(1e9, 1e9));
    }
    if (sec > 30000) {
        getContext().mMusic->stop();
        requestStackPush(States::Results);
        if (getContext().mGameData->mSurvivingPlayer) {
            ++getContext().mGameData->mPointsP2;
            getContext().mGameData->mWinningPlayer = 1;
        }
        else {
            ++getContext().mGameData->mPointsP1;
            getContext().mGameData->mWinningPlayer = 0;
        }
        getContext().mGameData->mSurvivingPlayer = 1-getContext().mGameData->mSurvivingPlayer;
    }
    else if (mPlayer->isDead()) {
        getContext().mMusic->stop();
        requestStackPush(States::Results);
        if (getContext().mGameData->mSurvivingPlayer) {
            ++getContext().mGameData->mPointsP1;
            getContext().mGameData->mWinningPlayer = 0;
        }
        else {
            ++getContext().mGameData->mPointsP2;
            getContext().mGameData->mWinningPlayer = 1;
        }
        getContext().mGameData->mSurvivingPlayer = 1-getContext().mGameData->mSurvivingPlayer;
    }
    mSceneGraph.update(dt);
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    if (mHasGamepads && (sf::Joystick::isConnected(mGamepad1) || sf::Joystick::isConnected(mGamepad2))) {
        int survival = getContext().mGameData->mSurvivingPlayer;
        int attacking = 1-survival;
        std::map<int, int> translateGamepads;
        translateGamepads[std::min(survival,attacking)] = mGamepad1;
        translateGamepads[std::max(survival,attacking)] = mGamepad2;
        if (event.type == sf::Event::JoystickButtonPressed) {
            std::cout << "joystick button pressed!" << std::endl;
            std::cout << "joystick id: " << event.joystickButton.joystickId << std::endl;
            std::cout << "button: " << event.joystickButton.button << std::endl;
        }

        if (event.type == sf::Event::JoystickButtonPressed) {
            if ((int)event.joystickButton.joystickId == translateGamepads[survival]) {
                // Jump
                if (!mPlayer->isDead()) {
                    if (event.joystickButton.button == 0) {
                        if (mPlayer->canJump()) {
                            getContext().mSound->play(SoundEffect::Jump);
                            mPlayer->jump(-mJumpVel);
                        }
                    }
                }
            }
            else {
                if (event.joystickButton.button == 0) {
                    activateTrap(Traps::Boxes);
                }
                if (event.joystickButton.button == 1) {
                    activateTrap(Traps::SpikesBall);
                }
                if (event.joystickButton.button == 2) {
                    activateTrap(Traps::Spikes);
                }
                if (event.joystickButton.button == 3) {
                    activateTrap(Traps::Platform);
                }
            }
        }
        else if (event.type == sf::Event::JoystickMoved) {
            if (event.joystickMove.axis == sf::Joystick::X) {
                std::cout << "X axis moved!" << std::endl;
                std::cout << "joystick id: " << event.joystickMove.joystickId << std::endl;
                std::cout << "new position: " << event.joystickMove.position << std::endl;
            }
        }
    }
    else {
        if (event.type == sf::Event::KeyPressed) {
            if (!mPlayer->isDead()) {
                if (event.key.code == sf::Keyboard::W) {
                    if (mPlayer->canJump()) {
                        getContext().mSound->play(SoundEffect::Jump);
                        mPlayer->jump(-mJumpVel);
                    }
                }
            }
            if (event.key.code == sf::Keyboard::Num7) {
                activateTrap(Traps::Boxes);
            }
            if (event.key.code == sf::Keyboard::Num8) {
                activateTrap(Traps::SpikesBall);

            }
            if (event.key.code == sf::Keyboard::Num9) {
                activateTrap(Traps::Spikes);

            }
            if (event.key.code == sf::Keyboard::Num0) {
                activateTrap(Traps::Platform);
            }
        }
    }
    
    return true;
}

void GameScreen::handleRealtimeInput(){
    if (!mPlayer->isDead()) {
        float speed = 10.f;
        sf::Vector2f multivel(0.0,0.0);
        

        if (mHasGamepads && (sf::Joystick::isConnected(mGamepad1) || sf::Joystick::isConnected(mGamepad2))) {
            int survival = getContext().mGameData->mSurvivingPlayer;
            int attacking = 1-survival;

            std::map<int, int> translateGamepads;
            translateGamepads[std::min(survival,attacking)] = mGamepad1;
            translateGamepads[std::max(survival,attacking)] = mGamepad2;

            float position = sf::Joystick::getAxisPosition(translateGamepads[survival], sf::Joystick::X);
            if(position < -20) {
                mPlayer->setLookingRight(false);
                mPlayer->setVel(mMovVel*position/100.0f,0.0f);
            }
            else if(position > 20) {
                mPlayer->setLookingRight(true);
                mPlayer->setVel(mMovVel*position/100.0f,0.0f);
            }

            float positionX = sf::Joystick::getAxisPosition(translateGamepads[attacking], sf::Joystick::X);
            float positionY = sf::Joystick::getAxisPosition(translateGamepads[attacking], sf::Joystick::Y);

            if(positionY < -20) {
                multivel.y+=speed*positionY/100.0f;
            }
            if(positionY > 20) {
                multivel.y+=speed*positionY/100.0f;
            }
            if(positionX < -20) {
                multivel.x+=speed*positionX/100.0f;
            }
            if(positionX > 20) {
                multivel.x+=speed*positionX/100.0f;
            }

        }
        else {
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
        }
        
        sf::Vector2f pos = mCursor->getWorldPosition();
        if (pos.x + multivel.x > 1820 || pos.x + multivel.x < 100) {
            mCursor->setPosition(pos);
        } else if (pos.y + multivel.y > 980 || pos.y + multivel.y < 100) {
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
            std::unique_ptr<SpriteNode> trap(new SpriteNode(trapTexture, CollisionType::SpikesBall));
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

void GameScreen::activateTrap(Traps::Traps trap) {
    switch(trap) {
        case Traps::Boxes:
        {
            float elapsedTime = mTrapButtons[Traps::Boxes]->getElapsedTime();
            if (elapsedTime > mTrapCooldown[Traps::Boxes]) {
                getContext().mSound->play(SoundEffect::Cage);
                addTrap(Traps::Boxes, mCursor->getWorldPosition());
                mTrapButtons[Traps::Boxes]->resetAnimation();
            }
        }
        break;

        case Traps::SpikesBall:
        {
            float elapsedTime = mTrapButtons[Traps::SpikesBall]->getElapsedTime();
            if (elapsedTime > mTrapCooldown[Traps::SpikesBall]) {
                getContext().mSound->play(SoundEffect::Spikeball);
                addTrap(Traps::SpikesBall, mCursor->getWorldPosition());
                mTrapButtons[Traps::SpikesBall]->resetAnimation();
            }
        }
        break;

        case Traps::Spikes:
        {
            float elapsedTime = mTrapButtons[Traps::Spikes]->getElapsedTime();
            if (elapsedTime > mTrapCooldown[Traps::Spikes]) {
                getContext().mSound->play(SoundEffect::Cage);
                addTrap(Traps::Spikes, mCursor->getWorldPosition());
                mTrapButtons[Traps::Spikes]->resetAnimation();
            }
        }
        break;

        case Traps::Platform:
        {
            getContext().mSound->play(SoundEffect::Switch);
            for (auto pltf : mPlatforms) {
                pltf->changeVisibility();
            }
        }
        break;

        default:
        break;

    }

}
