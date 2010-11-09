#include "XBOX360Controller.h"

bool XBOX360Controller::buttonWasReleased[BUTTON_COUNT] = {false};
void (XBOX360Controller::*onButtonPressed[BUTTON_COUNT])(void *) = {NULL};
void (XBOX360Controller::*onButtonReleased[BUTTON_COUNT])(void *) = {NULL};

bool XBOX360Controller::axisWasReleased[AXIS_COUNT] = {false};
void (XBOX360Controller::*onAxisMoved[AXIS_COUNT])(void *) = {NULL};
void (XBOX360Controller::*onAxisReleased[AXIS_COUNT])(void *) = {NULL};


float XBOX360Controller::getDeadZone() {
    return deadZone_;
}

void XBOX360Controller::setDeadZone(float deadZone) {
    deadZone_ = deadZone;
}

void XBOX360Controller::setCallBack(Button button, ButtonState state, void (*function)(void* ), void * pointer) {
    if (state == PRESSED) {
        onButtonPressed[button] = function;
        onButtonPressedPointers[button] = pointer;
    }
    else {
        onButtonReleased[button] = function;
        onButtonReleasedPointers[button] = pointer;
    }
}


void XBOX360Controller::setCallBack(Axis axis, ButtonState state, void (*function)(void *), void * pointer) {
    if (state == PRESSED) {
        onAxisMoved[axis] = function;
        onAxisMovedPointers[axis] = pointer;
    }
    else {
        onAxisReleased[axis] = function;
        onAxisReleasedPointers[axis] = pointer;
    }
}

void XBOX360Controller::listenButtons() {
    for (int button = 0; button < BUTTON_COUNT; button++)
        if (GetJoystickState().IsButtonPressed(button)) {

            if (onButtonPressed[button] != NULL)
                (*onButtonPressed[button])(onButtonPressedPointers[button]);

            buttonWasReleased[button] = true;
        }
        else
            if (buttonWasReleased[button]) {

                if (onButtonReleased[button] != NULL)
                    (*onButtonReleased[button])(onButtonReleasedPointers[button]);

                buttonWasReleased[button] = false;
            }
}

void XBOX360Controller::listenAxis() {}

bool XBOX360Controller::OnEvent(const SEvent& event) {
    if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
        && event.JoystickEvent.Joystick == 0) {

        joystickState_ = event.JoystickEvent;
        listenButtons();
        listenAxis();

        return true;
    }
    else
        return false;
}

const SEvent::SJoystickEvent& XBOX360Controller::GetJoystickState(void) const {
    return joystickState_;
}

XBOX360Controller::XBOX360Controller(float deadZone) {
    deadZone_ = deadZone;
}

XBOX360Controller::~XBOX360Controller() {
    //dtor
}
