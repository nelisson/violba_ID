#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cstdlib>
#include <ctime>

int sinal (double num);

int randomBetween(int first, int second);

void sleep(int seconds);

float startClock();

float timeDifference();

float timeDifference(clock_t start, clock_t end);

#endif // UTILS_H_INCLUDED
