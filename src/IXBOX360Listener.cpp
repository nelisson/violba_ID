#include "IXBOX360Listener.h"

bool IXBOX360Listener::buttonWasReleased[] = {false};
//void (IXBOX360Listener::*onButtonPressed[])  = {NULL}; // WTF???!?
// TODO (Juliano#1#): NAO CONSIGO INICIALIZAR ESSE ARRAY DE MERDA TT


bool IXBOX360Listener::axisWasReleased[] = {false};
//void IXBOX360Listener::(*onAxis[STATE_COUNT][AXIS_COUNT])    = {NULL};


void IXBOX360Listener::setCallBack(Button button, ButtonState state, void (*function)) {
    if (state == PRESSED)
        onButtonPressed[button] = function;
    else
        onButtonReleased[button] = function;
}


void IXBOX360Listener::setCallBack(Axis axis, ButtonState state, void (*function)) {
    if (state == PRESSED)
        onAxisMoved[axis] = function;
    else
        onAxisReleased[axis] = function;
}


void IXBOX360Listener::listenButtons() {}
void IXBOX360Listener::listenAxis() {}


IXBOX360Listener::IXBOX360Listener() {
    for (int button = 0; button < BUTTON_COUNT; button++) {
        onButtonPressed[button] = NULL;
        onButtonReleased[button] = NULL;
    }

    for (int axis = 0; axis < AXIS_COUNT; axis++) {
        onAxisMoved[axis] = NULL;
        onAxisReleased[axis] = NULL;
    }
}

IXBOX360Listener::~IXBOX360Listener() {
    //dtor
}
