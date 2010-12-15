#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>


int sinal (double num);

int randomBetween(int first, int second);

void sleep(int seconds);

float startClock();

float timeDifference();

float timeDifference(clock_t start, clock_t end);

const wchar_t* toWchar_T(long value);

const wchar_t* toWchar_T(int value);

const wchar_t* toWchar_T(float value);



#endif // UTILS_H_INCLUDED
