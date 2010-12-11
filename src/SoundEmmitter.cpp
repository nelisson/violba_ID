#include "SoundEmmitter.h"

bool FadeInfo::fade() {
    cout << "Entered fading" << endl;

    time_t currentTime;
    time(&currentTime);
    cout << "CurrentTime = " << currentTime <<  endl;
    time_t elapsedTimeFromLastUpdate = difftime(currentTime, lastTime_);
    cout << "ElapsedTimeFromLastUpdate = " << elapsedTimeFromLastUpdate <<  endl;

    lastTime_ = currentTime;
    cout << "LastTIme = " << lastTime_ <<  endl;

    time_t elapsedTimeFromStart = difftime(lastTime_, startTime_);
    cout << "ElapsedTimeFromStart = " << elapsedTimeFromStart <<  endl;

    cout <<"Will try to set volume" << endl;

    if ( elapsedTimeFromStart > fadeTimeMilliseconds_ / 1000.f ) {
        cout <<"Needs to finish sound" << endl;
        sound_->setVolume(fadeType_);
        cout <<"Set volume to limit" << endl;
        return false;
    }
    else {
        int volumeDelta = (fadeType_ == IN) ? 1 : -1;
        volumeDelta *= elapsedTimeFromLastUpdate / (float) (fadeTimeMilliseconds_ / 1000.f);

        cout <<"Volume delta: " << volumeDelta << endl;

        sound_->setVolume(sound_->getVolume() + volumeDelta);
        return true;
    }
}


void SoundEmmitter::loadSound(std::string soundPath, SoundType type) {
    sounds_[type].push_back(soundEngine_->getSoundSource(soundPath.data(), true));
}

void SoundEmmitter::loadSounds(vector<std::string> soundPaths, SoundType type){
    vector<string>::iterator i;
    for (i = soundPaths.begin(); i < soundPaths.end(); i++)
        loadSound(*i, type);
}

ISound* SoundEmmitter::playSound(int sound, SoundType type, bool looped, bool track) {
    std::cout<<"Attempting to play sound" << endl;
    return soundEngine_->play2D(sounds_[type].at(sound), looped, false, track);
}

void SoundEmmitter::refreshSounds() {
    vector<FadeInfo>::iterator i;
    for (i = fading_.begin(); i < fading_.end(); i++)
        if (i->fade())
            i = (fading_.erase(i))--;
}

void SoundEmmitter::loadSoundEffect(std::string soundEffect) {
    loadSound(soundEffect, SFX);
}

void SoundEmmitter::loadSoundEffects(vector<std::string> soundEffects) {
    vector<std::string>::iterator i;
    for (i = soundEffects.begin(); i < soundEffects.end(); i++)
        loadSoundEffect(*i);
}

void SoundEmmitter::playSoundEffect(int sound) {
    playSound(sound, SFX, false, false);
}

void SoundEmmitter::loadMusic(std::string music) {
    loadSound(music, MUSIC);
}

void SoundEmmitter::loadMusic(vector<std::string> music) {
    vector<std::string>::iterator i;
    for (i = music.begin(); i < music.end(); i++)
        loadMusic(*i);
}

void SoundEmmitter::playMusic(int music, bool fading, bool loop,
                              int fadeTimeMilliseconds) {

    if (!soundEngine_->isCurrentlyPlaying(sounds_[MUSIC].at(music))) {

        time_t startTime;
        time(&startTime);

        //stopMusic(true, 3000);
        musicPlaying_ = playSound(music, MUSIC, loop, fading ? true : false);

        if (fading) {
            musicPlaying_->setVolume(0);
            fading_.push_back(FadeInfo(musicPlaying_, fadeTimeMilliseconds, IN));
        }
    }
}

void SoundEmmitter::stopMusic(bool fading, int fadeTimeMilliseconds) {
    if (fading)
        fading_.push_back(FadeInfo(musicPlaying_,
                                   fadeTimeMilliseconds, OUT));
    else
        soundEngine_->removeSoundSource(musicPlaying_->getSoundSource());
}

SoundEmmitter::SoundEmmitter(ISoundEngine* soundEngine,
                             vector<std::string> sounds,
                             vector<std::string> music)
    : soundEngine_(soundEngine) {

    musicPlaying_ = NULL;
    loadSoundEffects(sounds);
    loadMusic(music);
}

SoundEmmitter::~SoundEmmitter() {
}

