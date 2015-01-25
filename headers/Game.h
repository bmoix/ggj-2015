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
#include "SoundPlayer.h"

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

        sf::RenderWindow mWindow;
        sf::Vector2u mWindowSize;
        sf::Vector2f mScale;
        
        sf::RenderTexture mRenderTexture;

        FontHolder mFonts;
        TextureHolder mTextures;
        MusicPlayer mMusic;
        SoundPlayer mSound;
        GameData mGameData;
        StatesStack mStatesStack;
        
        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;
        std::size_t mStatisticsNumFrames;
};


#endif // GAME_H
