/***********************Game.h****************************/
#ifndef GAME_H
#define GAME_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "StatesStack.h"
#include "StateIdentifiers.h"

#include "TitleScreen.h"
#include "GameScreen.h"
#include "CountdownScreen.h"
#include "HelpScreen.h"
#include "ResultsScreen.h"
#include "MusicPlayer.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


class Game {

    public:
        Game();

        int play();

    private:
        
        void render();
        void processInput();
        void registerStates();
        void update(sf::Time dt);
        void updateStatistics(sf::Time dt);

        static const sf::Time TimePerFrame;

        sf::Vector2f mScale;
        sf::Vector2u mWindowSize;
        sf::RenderWindow mWindow;
        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;
        sf::RenderTexture mRenderTexture;

        FontHolder mFonts;
        StatesStack mStatesStack;
        TextureHolder mTextures;
        MusicPlayer mMusic;
        GameData mGameData;
        std::size_t mStatisticsNumFrames;
};


#endif // GAME_H
