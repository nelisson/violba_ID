#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <irrlicht/irrlicht.h>

using namespace irr::core;


namespace NodeIDFlags {
    enum NodeIDFlags {
        IGNORED = 0,
        ENEMY = 1,
        ITEM = 1 << 1,
        CORPSE = 1 << 2,
        FLOOR = 1 << 3,
    };
}

namespace ControllerType{
    enum ControllerType{
        XBOX360,
        PLAYSTATION2,
        };
}


int sinal (double num);

int randomBetween(int first, int second);

void sleep(int seconds);

float startClock();

float timeDifference();

float timeDifference(clock_t start, clock_t end);

const wchar_t* toWchar_T(long value,bool bar = false);

const wchar_t* toWchar_T(int value, bool bar = false);

const wchar_t* toWchar_Kills(int value);

const wchar_t* toWchar_T(float value, bool bar = false);

const wchar_t* toWchar_T(vector3df position);

recti getStringSize(const char* value, int font);

recti getStringSize(char* value, int font);

#endif // UTILS_H_INCLUDED
