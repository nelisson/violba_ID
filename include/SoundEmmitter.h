#ifndef SOUNDEMMITTER_H
#define	SOUNDEMMITTER_H

#include <irrklang/irrKlang.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <ctime>
#include <vector>
#include <set>

#define DEFAULT_FADE_TIME 5

using namespace irrklang;
using namespace std;

enum FadeType {
    OUT, IN,
};

enum SoundType {
    SFX, MUSIC,
};

class FadeInfo {
    private:
        ISound* sound_;
        clock_t startTime_;
        clock_t lastTime_;
        int fadeTime_;
        FadeType fadeType_;

    public:
        bool fade();

        FadeInfo(ISound* sound,
                 int fadeTime,
                 FadeType fadeType)
            : sound_(sound),
              fadeTime_(fadeTime),
              fadeType_(fadeType) {

            startTime_ = clock();
            lastTime_ = clock();
        }
};


class SoundEmmitter {
    private:
        ISoundEngine* soundEngine_;

        vector<std::string> sounds_[2];

        void addSound(std::string sound, SoundType type) { sounds_[type].push_back(sound); }
        void addSounds(vector<std::string> sounds, SoundType type);

        FadeInfo* fading_[2];
        ISound* musicPlaying_;
        std::string currentMusic_;

    protected:

    public:
        ISound * getCurrentMusic() const { return musicPlaying_; }

        ISoundEngine* getSoundEngine() const { return soundEngine_; }

        void refreshSounds();

        void addSoundEffect(std::string sound)           { addSound(sound, SFX); }
        void addSoundEffects(vector<std::string> sounds) { addSounds(sounds, SFX); }

        void addMusic(std::string music)         { addSound(music, MUSIC); }
        void addMusic(vector<std::string> music) { addSounds(music, MUSIC); }

        void playSoundEffect(int sound, bool looped = false);
        void stopSoundEffect(int sound);

        void playMusic(int music, bool fading = false, bool loop = true,
                       int fadeTime = DEFAULT_FADE_TIME);

        void stopMusic(bool fading = true,
                       int fadeTime = DEFAULT_FADE_TIME);
        
        SoundEmmitter(ISoundEngine* soundEngine);

        virtual ~SoundEmmitter();
};
#endif	/* SOUNDEMMITTER_H */