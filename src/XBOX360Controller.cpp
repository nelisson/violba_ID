#include "XBOX360Controller.h"

bool XBOX360Controller::buttonWasReleased[BUTTON_COUNT] = {false};
void (XBOX360Controller::*onButtonPressed[BUTTON_COUNT])(void *) = {NULL};
void (XBOX360Controller::*onButtonReleased[BUTTON_COUNT])(void *) = {NULL};

bool XBOX360Controller::axisWasReleased[AXIS_COUNT] = {false};
void (XBOX360Controller::*onAxisMoved[AXIS_COUNT])(void * classPointer, core::vector2df direction) = {NULL};
void (XBOX360Controller::*onAxisReleased[AXIS_COUNT])(void * classPointer, core::vector2df direction) = {NULL};


float XBOX360Controller::getDeadZone() {
    return deadZone_;
}

void XBOX360Controller::setDeadZone(float deadZone) {
    deadZone_ = deadZone;
}

core::vector2df XBOX360Controller::getLAnalog() {
    return core::vector2df(lAnalogX, lAnalogY);
}

float XBOX360Controller::getLTrigger() {
    return lTrigger;
}

float XBOX360Controller::getRTrigger() {
    return rTrigger;
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


void XBOX360Controller::setCallBack(Axis axis, ButtonState state, void (*function)(void *, core::vector2df), void * pointer) {
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

void XBOX360Controller::listenAxis() {

    float x = fixInput(GetJoystickState().Axis[L_ANALOG]);
    float y = fixInput(GetJoystickState().Axis[L_ANALOG + 1]);

    float trigger = fixInput(GetJoystickState().Axis[LT]);

    for (int axis = 0; axis < AXIS_COUNT; axis++) {
        if (anyAxisMoved()) {

            if (onAxisMoved[axis] != NULL)
                (*onAxisMoved[axis])(onAxisMovedPointers[axis], getLAnalog());

            axisWasReleased[axis] = true;
        }
        else
            if (axisWasReleased[axis]) {

                if (onAxisReleased[axis] != NULL)
                    (*onAxisReleased[axis])(onAxisReleasedPointers[axis], getLAnalog());

                axisWasReleased[axis] = false;
            }
    }
}

bool XBOX360Controller::anyAxisMoved() {
    return lAnalogX || lAnalogY;
}

float XBOX360Controller::fixInput(float x) {
    return (x < getDeadZone()) ? 0 : x;
}

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
