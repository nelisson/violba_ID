#include "Utils.h"
#include "MainCharacter.h"

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

const wchar_t* toWchar_T(long value, bool bar){
    wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t)*15);
    char* buffer1 = (char*)malloc(sizeof(char)*15);

    if(bar)
        sprintf(buffer1, "%ld/",value);
    else
        sprintf(buffer1, "%ld",value);

    mbstowcs(buffer, buffer1, strlen(buffer1)+1);

    return buffer;
}

const wchar_t* toWchar_T(int value, bool bar){
    wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t)*15);
    char* buffer1 = (char*)malloc(sizeof(char)*15);

    if(bar)
        sprintf(buffer1, "%d/",value);
    else
        sprintf(buffer1, "%d",value);

    mbstowcs(buffer, buffer1, strlen(buffer1)+1);

    return buffer;
}

const wchar_t* toWchar_T(float value, bool bar){
    wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t)*15);
    char* buffer1 = (char*)malloc(sizeof(char)*15);

    if(bar)
        sprintf(buffer1, "%.0f/",value);
    else
        sprintf(buffer1, "%.0f",value);

    mbstowcs(buffer, buffer1, strlen(buffer1)+1);

    return buffer;
}


recti getStringSize(const char* value, int font){

    int lenght = strlen(value);
    int x,y;

    x = lenght*font;
    y = (int)((float)11/9*(font)) ;

    return rect<s32> (0,0,x,y);
}


recti getStringSize(char* value, int font){

    int lenght = strlen(value);
    int x,y;

    x = lenght*font;
    y = (int)((float)11/9*(font)) ;

    return rect<s32> (0,0,x,y);
}