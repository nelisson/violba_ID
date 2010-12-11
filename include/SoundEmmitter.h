#ifndef SOUNDEMMITTER_H
#define	SOUNDEMMITTER_H

#include <irrklang/irrKlang.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <ctime>
#include <vector>

#define DEFAULT_FADE_TIME_MILLISECONDS 2000

using namespace irrklang;
using namespace std;


enum SoundType {
    SFX,
    MUSIC,
};


enum FadeType {
    OUT,
    IN,
};


class FadeInfo {
    private:
        ISound* sound_;
        time_t startTime_;
        time_t lastTime_;
        int fadeTimeMilliseconds_;
        FadeType fadeType_;

    public:
        bool fade();

        FadeInfo(ISound* sound,
                 int fadeTimeMilliseconds,
                 FadeType fadeType)
            : sound_(sound),
              fadeTimeMilliseconds_(fadeTimeMilliseconds),
              fadeType_(fadeType) {

            time(&startTime_);
            time(&lastTime_);
        }
};


class SoundEmmitter {
    private:
        ISoundEngine* soundEngine_;

        vector<ISoundSource*> sounds_[2];

        vector<FadeInfo> fading_;
        ISound* musicPlaying_;

        ISound* playSound(int sound, SoundType type,
                          bool looped = false, bool track = false);

    protected:

    public:
        ISoundEngine * getSoundEngine() { return soundEngine_; }

        ISound * getCurrentMusic() { return musicPlaying_; }

        void loadSound(std::string soundPath, SoundType type);
        void loadSounds(vector<std::string> soundPaths, SoundType type);

        void refreshSounds();

        void loadSoundEffect(std::string soundEffect);
        void loadSoundEffects(vector<std::string> soundEffects);
        void playSoundEffect(int sound);

        void loadMusic(std::string music);
        void loadMusic(vector<std::string> music);
        void playMusic(int music, bool fading = false, bool loop = true,
                       int fadeTimeMilliseconds = DEFAULT_FADE_TIME_MILLISECONDS);

        void stopMusic(bool fading, int fadeTimeMilliseconds);
        
        SoundEmmitter(ISoundEngine* soundEngine,
                      vector<std::string> sounds = vector<std::string>(),
                      vector<std::string> music  = vector<std::string>());

        virtual ~SoundEmmitter();
};

#endif	/* SOUNDEMMITTER_H */

