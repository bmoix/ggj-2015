#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
: mSoundBuffer()
, mSounds() {
    mSoundBuffer.load(SoundEffect::Cage, "res/sounds/cage.wav");
    mSoundBuffer.load(SoundEffect::Gong, "res/sounds/gong.wav");
    mSoundBuffer.load(SoundEffect::Jump, "res/sounds/jump.wav");
    mSoundBuffer.load(SoundEffect::Spikeball, "res/sounds/spikeball.wav");
    mSoundBuffer.load(SoundEffect::Spikes, "res/sounds/spikes.wav");
    mSoundBuffer.load(SoundEffect::Switch, "res/sounds/switch.ogg");
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
