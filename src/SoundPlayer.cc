#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
: mSoundBuffer()
, mSounds() {
    //mSoundBuffers.load(SoundEffect::WOLOLO, "res/");
}

void SoundPlayer::play(SoundEffect::ID effect) {
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();
    sound.setBuffer(mSoundBuffer.get(effect));
    sound.play();
}

void SoundPlayer::removeStoppedSounds() {
    mSounds.remove_if([] (const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}
