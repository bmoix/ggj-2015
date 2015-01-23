/*********************************TitleScreen.cc***********************************/
#include "TitleScreen.h"

TitleScreen::TitleScreen(StatesStack& stack, Context context)
: State(stack, context) {
}

void TitleScreen::draw() {

}
bool TitleScreen::update(sf::Time dt) {
    return true;
}

bool TitleScreen::handleEvent(const sf::Event& event) {
    return true;
}
