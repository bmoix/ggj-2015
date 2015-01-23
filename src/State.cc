#include "State.h"
#include "StatesStack.h"

State::Context::Context(sf::RenderTexture& rTexture, TextureHolder& textures, FontHolder& fonts, sf::Vector2f Scale)
: mRTexture(&rTexture)
, mTextures(&textures)
, mFonts(&fonts)
, mScale(Scale) {
}

State::State(StatesStack& stack, Context context)
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
