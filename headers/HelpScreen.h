/********************************HelpScreen.h***************************************/

#ifndef HELP_SCREEN_H
#define HELP_SCREEN_H

#include "Utils.h"
#include "State.h"
#include "SceneNode.h"
#include "TextNode.h"
#include "SpriteNode.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

class HelpScreen: public State {

    public:
        //Constructor with name of the image it want's to display
        HelpScreen(StatesStack& stack, Context& context);

        void draw();
        bool update(sf::Time dt);
        bool handleEvent(const sf::Event& event);

    private:
        void click(mouseButtons mouseButton, sf::Vector2f mouseClick);
        bool contains(const sf::FloatRect& rect, sf::Vector2i pos);
        enum Layer {
            Background,
            Text,
            LayerCount
        };

        SceneNode mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        SpriteNode* mButtonBack;
        TextNode* mText;
        TextNode* mText2;
        TextNode* mTextWat;

        std::string str;
};


#endif // HELP_SCREEN_H
