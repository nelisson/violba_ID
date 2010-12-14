#include "Utils.h"

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
