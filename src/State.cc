#include "State.h"
#include "StatesStack.h"



State::Context::Context(sf::RenderTexture& rTexture, TextureHolder& textures, 
    FontHolder& fonts, sf::Vector2f Scale, sf::RenderWindow& window, MusicPlayer& music, 
    SoundPlayer& sound, GameData& gameData)
: mRTexture(&rTexture)
, mTextures(&textures)
, mFonts(&fonts)
, mScale(Scale)
, mWindow(&window)
, mMusic(&music)
, mSound(&sound)
, mGameData(&gameData) {
}

State::State(StatesStack& stack, Context& context)
: mStack(&stack)
, mContext(context) {
}

State::~State() {
}

void State::requestStackPush(States::ID IDState) {
    mStack->pushState(IDState);
}

void State::requestStackPop() {
    mStack->popState();
}

void State::requestStackClear() {
    mStack->clearStates();
}

State::Context State::getContext() const {
    return mContext;
}
