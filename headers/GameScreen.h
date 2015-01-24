/********************************GameScreen.h***************************************/

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "Utils.h"
#include "State.h"
#include "SceneNode.h"
#include "TextNode.h"
#include "Player.h"
#include "SpriteNode.h"
#include "InvisibleNode.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

enum class Traps {
    Boxes,
    SpikesBall,
    Spikes,
    Platform,
};

class GameScreen: public State {

    public:
        //Constructor with name of the image it want's to display
        GameScreen(StatesStack& stack, Context context);

        void draw();
        bool update(sf::Time dt);
        bool handleEvent(const sf::Event& event);

        void addTrap(int type, sf::Vector2f pos);

    private:
        void handleRealtimeInput();
        void handleCollisions();
        bool checkCollision(SpriteNode* a, SpriteNode* b);
        void click(mouseButtons mouseButton, sf::Vector2f mouseClick);

        enum Layer {
            Background,
            World,
            Players,
            Traps,
            Text,
            LayerCount
        };

        SceneNode mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;
        std::vector<SpriteNode*> mWalls;
        SpriteNode* mGround;
        std::vector<SpriteNode*> mTrapButtons;
        std::vector<int> mTrapsAvailable;
        std::vector<TextNode*> mTextTraps;

        b2World* mWorld;

        TextNode* mText;

        Player* mPlayer;
        float mJumpVel;
        float mMovVel;

        std::vector<SpriteNode*> mTraps;

        std::string str;
};


#endif // GAME_SCREEN_H
