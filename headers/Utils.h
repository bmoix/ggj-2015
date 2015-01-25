#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <list>
#include <cmath>
#include <array>
#include <cstdio>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>
#include <cassert>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <Box2D/Box2D.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct GameData {
    GameData(
        int numRounds, 
        int roundsPassed, 
        int pointsP1, 
        int pointsP2, 
        int survivingPlayer,
        int winningPlayer
    );
    int mNumRounds;
    int mRoundsPassed;
    int mPointsP1;
    int mPointsP2;
    int mSurvivingPlayer;
    int mWinningPlayer;
};

inline GameData::GameData(
    int numRounds,
    int roundsPassed,
    int pointsP1,
    int pointsP2,
    int survivingPlayer,
    int winningPlayer) :
    mNumRounds(numRounds),
    mRoundsPassed(roundsPassed),
    mPointsP1(pointsP1),
    mPointsP2(pointsP2),
    mSurvivingPlayer(survivingPlayer),
    mWinningPlayer(winningPlayer) {
}  

const float metersToPixels = 192.0f;
const float epsilon = 1e-2f;

const float bbscalex_p1 = 0.70f;
const float bbscaley_p1 = 0.70f;

const float bbscalex_p2 = 0.44f;
const float bbscaley_p2 = 0.60f;

enum directions {
    dir_up, dir_down, dir_right, dir_left, dir_up_right, dir_down_left, dir_up_left, dir_down_right, dir_none
};

enum mouseButtons {
    mouse_left, mouse_right, mouse_rodeta, mouse_none
};

//                      N   S   E   W    NE     SW    NW   SE  NONE
const float dirx[9] = { 0,  0,  1, -1,  0.5,  -0.5, -0.5, 0.5,   0};
const float diry[9] = {-1,  1,  0,  0, -0.5,   0.5, -0.5, 0.5,   0};

const float PlayerSpeed = 500.f;
const sf::Time TimePerFrame = sf::seconds(1.f/60.f); // 60 fps

const sf::Vector2u gameSize(1920,1080);

namespace Utils {
    inline std::wstring utf8_to_utf16(const std::string& utf8) {
        std::vector<unsigned long> unicode;
        size_t i = 0;
        while (i < utf8.size()) {
            unsigned long uni; size_t todo;
            unsigned char ch = utf8[i++];
            if (ch <= 0x7F) { uni = ch; todo = 0; }
            else if (ch <= 0xBF) { throw std::logic_error("not a UTF-8 string"); }
            else if (ch <= 0xDF) { uni = ch&0x1F; todo = 1; }
            else if (ch <= 0xEF) { uni = ch&0x0F; todo = 2; }
            else if (ch <= 0xF7) { uni = ch&0x07; todo = 3; }
            else {  throw std::logic_error("not a UTF-8 string"); }
            for (size_t j = 0; j < todo; ++j) {
                if (i == utf8.size()) throw std::logic_error("not a UTF-8 string");
                unsigned char ch = utf8[i++];
                if (ch < 0x80 || ch > 0xBF) throw std::logic_error("not a UTF-8 string");
                uni <<= 6; uni += ch & 0x3F;
            }
            if (uni >= 0xD800 && uni <= 0xDFFF) throw std::logic_error("not a UTF-8 string");
            if (uni > 0x10FFFF)                 throw std::logic_error("not a UTF-8 string");
            unicode.push_back(uni);
        }
        std::wstring utf16;
        for (size_t i = 0; i < unicode.size(); ++i) {
            unsigned long uni = unicode[i];
            if (uni <= 0xFFFF) { utf16 += (wchar_t)uni; }
            else { uni -= 0x10000;
                utf16 += (wchar_t)((uni >> 10) + 0xD800);
                utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00); }
        }
        return utf16;
    }

    inline void centerOrigin(sf::Sprite& sprite) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
    }

    inline void centerOrigin(sf::Text& text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
    }

    inline std::string getTexturePath(const std::string& name) {
        std::stringstream png, jpg;
        png << "res/pictures/" << name << ".png";
        jpg << "res/pictures/" << name << ".jpg";
        std::ifstream f(png.str());
        if (f.good()) {
            f.close();
            return png.str();
        }
        else {
            f.close();
            return jpg.str();
        }   
    }

    inline sf::Vector2i correctMouse(sf::Vector2i mousePos, sf::Vector2f Scale) {
        sf::Vector2i correct;
        correct.x = int(mousePos.x * 1.0/Scale.x);
        correct.y = int(mousePos.y * 1.0/Scale.y);
        return correct;
    }
}

#endif // UTILS_H
