#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "Utils.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class SoundPlayer : private sf::NonCopyable {
    public:
        SoundPlayer();
        void play(SoundEffect::ID effect);
        void removeStoppedSounds();

    private:
        SoundBufferHolder mSoundBuffer;
        std::list<sf::Sound> mSounds;
};

#endif // SOUND_PLAYER_H
