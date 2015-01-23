/***********************************main.cpp*************************************************/
#include "Game.h"

#include <stdexcept>
#include <iostream>

int main() {

    try {
        Game Game;
        return Game.play();
    }
    catch (std::exception& e) {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }


}
