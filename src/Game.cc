#include "Game.h"

#include "Utils.h"
#include "State.h"
#include "StateIdentifiers.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

//Constructor

Game::Game() :
  mWindow(sf::VideoMode(sf::VideoMode::getDesktopMode()), L"Kill Or Survive"
    , sf::Style::None/*sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize*/)
, mWindowSize(mWindow.getSize().x, mWindow.getSize().y)
, mScale(float(mWindow.getSize().x)/1920.0, 9.0/16.0*float(mWindow.getSize().x)/1920.0)
, mRenderTexture()
, mFonts()
, mTextures()
, mMusic()
, mSound()
, mGameData(1000,0,0,0,0,0) // Set to 1000 rounds for more fun
, mStatesStack(State::Context(mRenderTexture, mTextures, mFonts, mScale, mWindow, mMusic, mSound, mGameData))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    bool renderCreated = mRenderTexture.create(1920, 1080);
    assert(renderCreated);
    mRenderTexture.setSmooth(true);

    mWindowSize = mWindow.getSize();
    mScale = sf::Vector2f(float(mWindow.getSize().x)/1920.0, float(mWindow.getSize().x)/1920.0);
    mStatesStack.setContext(State::Context(mRenderTexture, mTextures, mFonts, mScale, mWindow, mMusic, mSound, mGameData));

    mWindow.setKeyRepeatEnabled(false);
	std::cout << 1 << std::endl;
    // Carrega recursos comuns
    mFonts.load(Fonts::AlluraRegular, "res/media/AlluraRegular.otf");
    mFonts.load(Fonts::Sansation, "res/media/Sansation.ttf");
    mFonts.load(Fonts::Gomo, "res/media/Gomo.ttf");
    mTextures.load(Textures::GameBackground1, "res/pictures/background1.png");
    mTextures.load(Textures::GameBackground2, "res/pictures/background2.png");
    mTextures.load(Textures::HelpBackground, "res/pictures/helpBackground.png");
    mTextures.load(Textures::CountdownBackground, "res/pictures/countdownBackground.png");
    mTextures.load(Textures::RaccoonWins, "res/pictures/raccoonWins.png");
    mTextures.load(Textures::FoxWins, "res/pictures/foxWins.png");
    mTextures.load(Textures::Player1, "res/pictures/playerAnimation1.png");
    mTextures.load(Textures::Player2, "res/pictures/playerAnimation2.png");
    mTextures.load(Textures::Blue, "res/pictures/blue.png");
    mTextures.load(Textures::Red, "res/pictures/red.png");
    mTextures.load(Textures::Platform1, "res/pictures/platform1.png");
    mTextures.load(Textures::TrapBox, "res/pictures/trap_box.png");
    mTextures.load(Textures::SpikesBall, "res/pictures/spikes_ball.png");
    mTextures.load(Textures::PlatformWood, "res/pictures/platformWood.png");
    mTextures.load(Textures::PlatformStone, "res/pictures/platformStone.png");
    mTextures.load(Textures::IconBox, "res/pictures/iconBox.png");
    mTextures.load(Textures::IconBall, "res/pictures/iconBall.png");
    mTextures.load(Textures::IconSpikes, "res/pictures/iconSpikes.png");
    mTextures.load(Textures::IconSwitch, "res/pictures/iconSwitch.png");
    mTextures.load(Textures::Spikes, "res/pictures/spikes.png");
    mTextures.load(Textures::Title, "res/pictures/titleScreen.png");
    mTextures.load(Textures::Start, "res/pictures/starticon.png");
    mTextures.load(Textures::Help, "res/pictures/helpicon.png");
    mTextures.load(Textures::Exit, "res/pictures/exiticon.png");
    mTextures.load(Textures::Cursor, "res/pictures/cursor.png");
    mTextures.load(Textures::Return, "res/pictures/return.png");
    mTextures.load(Textures::AButton, "res/pictures/abutton.png");
    mTextures.load(Textures::BButton, "res/pictures/bbutton.png");
    mTextures.load(Textures::XButton, "res/pictures/xbutton.png");
    mTextures.load(Textures::YButton, "res/pictures/ybutton.png");

    mStatisticsText.setFont(mFonts.get(Fonts::Sansation));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);
    mStatisticsText.setColor(sf::Color::Black);

    mWindow.setVerticalSyncEnabled(true);

    registerStates();
    mStatesStack.pushState(States::Title);
}

int Game::play() {

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
//std::cout << clock.getElapsedTime().asSeconds() << " initialBucle time" << std::endl;
            processInput();
//std::cout << clock.getElapsedTime().asSeconds() << " time to process Input " << std::endl;
            update(TimePerFrame);
//std::cout << clock.getElapsedTime().asSeconds() << " time to update" << std::endl;
            // Check inside this loop, because stack might be empty before update() call
            if (mStatesStack.isEmpty())
                mWindow.close();
        }
        updateStatistics(dt);
//std::cout << clock.getElapsedTime().asSeconds() << "let's render" << std::endl;
        render();
//std::cout << clock.getElapsedTime().asSeconds() << "rendered------------------------------------------------------------------------------------" << std::endl;
    }

    return EXIT_SUCCESS;
}

void Game::processInput() {
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStatesStack.handleEvent(event);

        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            mWindow.close();
        }
    }
}

void Game::update(sf::Time dt) {
    mStatesStack.update(dt);
}

void Game::render() {
    mRenderTexture.clear();

    mStatesStack.draw();

    mRenderTexture.display();

    // Now we start rendering to the window, clear it first
    mWindow.clear();
    mWindow.setView(mWindow.getDefaultView());
    // Draw the texture
    sf::Sprite sprite(mRenderTexture.getTexture());
    // Llegeix mida de la finestra (x, y)
    mWindowSize = mWindow.getSize();
    sprite.setScale(1.0, 1.0);
    sprite.setPosition(0.0, 0.0);
    mScale = sf::Vector2f(float(mWindowSize.x)/float(mRenderTexture.getSize().x), float(mWindowSize.x)/float(mRenderTexture.getSize().x)); // 16:9 aspect
    //std::cout << mScale.x << " " << mScale.y << std::endl;
    sprite.setScale(mScale);
    mWindow.draw(sprite);
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    // End the current frame and display its contents on screen
    mWindow.display();

}

void Game::updateStatistics(sf::Time dt) {
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Game::registerStates() {
    mStatesStack.registerState<TitleScreen>(States::Title);
    mStatesStack.registerState<HelpScreen>(States::Help);
    mStatesStack.registerState<GameScreen>(States::Game);
    mStatesStack.registerState<CountdownScreen>(States::Countdown);
    mStatesStack.registerState<ResultsScreen>(States::Results);
}

