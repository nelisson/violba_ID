#include "SoundEmmitter.h"
#include "Utils.h"

bool FadeInfo::fade() {
    clock_t currentTime = clock();

    lastTime_ = currentTime;

    float elapsedTimeFromStart = timeDifference(startTime_, lastTime_);

    if ( elapsedTimeFromStart > fadeTime_ ) {
        sound_->setVolume(fadeType_);

        if (fadeType_ == OUT)
            sound_->stop();

        return false;
    }
    else {
        float volume = elapsedTimeFromStart / (float)fadeTime_;

        if (fadeType_ == IN)
            sound_->setVolume(volume);
        else
            sound_->setVolume(1-volume);

        return true;
    }
}

void SoundEmmitter::refreshSounds() {
    if (fading_[IN])
        if (!fading_[IN]->fade())
            fading_[IN] = NULL;
        
    if (fading_[OUT])
        if (!fading_[OUT]->fade())
            fading_[OUT] = NULL;
}

void SoundEmmitter::addSounds(vector<std::string> sounds, SoundType type) {
    vector<std::string>::iterator i;
    for (i = sounds.begin(); i < sounds.end(); i++)
        addSound(*i, type);
}

void SoundEmmitter::playSoundEffect(int sound, bool looped) {
    soundEngine_->play2D(sounds_[SFX].at(sound).data(), looped, false, false);
}

void SoundEmmitter::stopSoundEffect(int sound) {
    soundEngine_->removeSoundSource(sounds_[SFX].at(sound).data());
}

void SoundEmmitter::playMusic(int music, bool fading, bool loop,
                              int fadeTime) {

    const char * musicString = sounds_[MUSIC].at(music).data();

    if ( currentMusic_ != musicString ) {

        cout<<"Trying to play a different music"<<endl;
        
        if (musicPlaying_) {
            delete fading_[OUT];
            fading_[OUT] = new FadeInfo(musicPlaying_, fadeTime, OUT);
        }

        currentMusic_.assign(musicString);
        musicPlaying_ = soundEngine_->play2D(musicString, loop, false, loop);

        cout<<"Playing music..."<<endl;

        if (fading) {
            delete fading_[IN];
            musicPlaying_->setVolume(0);
            fading_[IN] = new FadeInfo(musicPlaying_, fadeTime, IN);
        }
    }
}

void SoundEmmitter::stopMusic(bool fading, int fadeTimeMilliseconds) {
    if (musicPlaying_) {
        if (fading)
            fading_[OUT] = new FadeInfo(musicPlaying_, fadeTimeMilliseconds, OUT);
        else
            musicPlaying_->stop();

        musicPlaying_ = NULL;
    }
}

SoundEmmitter::SoundEmmitter(ISoundEngine* soundEngine)
    : soundEngine_(soundEngine) {

    musicPlaying_ = NULL;
    fading_[IN]   = NULL;
    fading_[OUT]  = NULL;
}

SoundEmmitter::~SoundEmmitter() {}

