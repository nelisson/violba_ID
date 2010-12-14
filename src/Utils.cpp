#include "Utils.h"

static clock_t start;

int sinal(double num) {
    return (num < 0) ? -1 : 1;
}

int randomBetween(int first, int second) {
    return rand() % (1 + second - first) + first;
}

void sleep(int seconds) {
    time_t start;
    time_t current;
    time(&start);

    do{
        time(&current);
    } while(difftime(current, start) < seconds);
}

float startClock() {
    return start = clock();
}

float timeDifference()
{
    return timeDifference(start, clock());
}

float timeDifference(clock_t start, clock_t end)
{
    return (float) (end - start) / (float)CLOCKS_PER_SEC;
}

