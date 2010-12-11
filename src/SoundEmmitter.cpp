#include "SoundEmmitter.h"
#include "Utils.h"

bool FadeInfo::fade() {
    cout << "Entered fading\n---------------" << endl;

    clock_t currentTime = clock();
    
    cout << "CurrentTime = " << currentTime <<  endl;
    float elapsedTimeFromLastUpdate = timeDifference(lastTime_, currentTime);
    cout << "ElapsedTimeFromLastUpdate = " << elapsedTimeFromLastUpdate <<  endl;

    cout << "LastTIme = " << lastTime_ <<  endl;
    lastTime_ = currentTime;

    float elapsedTimeFromStart = timeDifference(startTime_, lastTime_);
    cout << "ElapsedTimeFromStart = " << elapsedTimeFromStart <<  endl;
    cout << "FadeTime = " << fadeTime_ <<  endl;

    if ( elapsedTimeFromStart > fadeTime_ ) {
        cout <<"Needs to finish sound" << endl;
        sound_->setVolume(fadeType_);
        cout <<"Set volume to limit = " << fadeType_ << endl;


        if (fadeType_ == OUT);
            sound_->stop();

        return false;
    }
    else {
        float volume = (fadeType_ == IN) ? 1 : -1;
        volume *= elapsedTimeFromStart / (float)fadeTime_;

        cout <<"Volume delta: " << volume << endl;
                
        sound_->setVolume(volume);

        return true;
    }
}

void SoundEmmitter::refreshSounds() {
    cout<<"Refreshing Sounds"<<endl;
    if (fading_[IN]) {
        cout<<"Start Fading In"<<endl;
        if (!fading_[IN]->fade()) {
            cout<<"Ended Fading"<<endl;
            fading_[IN] = NULL;
        }
    }
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

    cout<<"Current Music: " << musicString << endl;

    if(musicPlaying_)
        cout << "Music returned: " << musicPlaying_->getSoundSource()->getName() << endl;

    if ( currentMusic_ != musicString ) {

        cout<<"Trying to play a different music"<<endl;
        
        if (musicPlaying_) {
            delete fading_[OUT];
            fading_[OUT] = new FadeInfo(musicPlaying_, fadeTime, IN);
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

