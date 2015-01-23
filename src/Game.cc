#include "Game.h"

#include "Utils.h"
#include "State.h"
#include "StateIdentifiers.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

//Constructor
Game::Game() :
  mWindow(sf::VideoMode(sf::VideoMode::getDesktopMode()), L"1714: La resistència de l'Història"
    , sf::Style::None/*sf::Style::Titlebar | sf::Style::Close*/)
, mRenderTexture()
, mTextures()
, mFonts()
, mWindowSize(mWindow.getSize().x, mWindow.getSize().y)
, mScale(float(mWindow.getSize().x)/1920.0, 9.0/16.0*float(mWindow.getSize().x)/1920.0)
, mStatesStack(State::Context(mRenderTexture, mTextures, mFonts, mScale))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0){
    bool renderCreated = mRenderTexture.create(1920, 1080);
    assert(renderCreated);
    mRenderTexture.setSmooth(true);

    mWindowSize = mWindow.getSize();
    mScale = sf::Vector2f(float(mWindow.getSize().x)/1920.0, float(mWindow.getSize().x)/1920.0);
    mStatesStack.setContext(State::Context(mRenderTexture, mTextures, mFonts, mScale));

    mWindow.setKeyRepeatEnabled(false);

    // Carrega recursos comuns
    mFonts.load(Fonts::AlluraRegular, "res/media/AlluraRegular.otf");
    mFonts.load(Fonts::Sansation, "res/media/Sansation.ttf");
    mTextures.load(Textures::GameBackground, "res/pictures/background.png");
    mTextures.load(Textures::Player1, "res/pictures/player1.png");

    mStatisticsText.setFont(mFonts.get(Fonts::Sansation));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);
    mStatisticsText.setColor(sf::Color::Black);

    mWindow.setVerticalSyncEnabled(true);

    registerStates();
    mStatesStack.pushState(States::Game);
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

        if (event.type == sf::Event::Closed)
            mWindow.close();

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
    mStatesStack.registerState<GameScreen>(States::Game);
}

