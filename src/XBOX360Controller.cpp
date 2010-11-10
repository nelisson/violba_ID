#include "XBOX360Controller.h"


bool XBOX360Controller::buttonWasReleased[BUTTON_COUNT] = {false};
bool XBOX360Controller::analogWasReleased[ANALOG_COUNT] = {false};
bool XBOX360Controller::triggerWasReleased[TRIGGER_COUNT] = {false};

void (XBOX360Controller::*onButton[STATE_COUNT][BUTTON_COUNT])(void *) = {NULL};
void (XBOX360Controller::*onAnalog[STATE_COUNT][ANALOG_COUNT])(void * classPointer, core::vector2df direction) = {NULL};
void (XBOX360Controller::*onTrigger[STATE_COUNT][TRIGGER_COUNT])(void * classPointer, float value) = {NULL};

float XBOX360Controller::getDeadZone() {
    return deadZone_;
}

void XBOX360Controller::setDeadZone(float deadZone) {
    deadZone_ = deadZone;
}

core::vector2df XBOX360Controller::getAnalog(Analog analog) {
    return analogs_[analog];
}

core::vector2df XBOX360Controller::getAnalog(int analog) {
    return analogs_[analog];
}

float XBOX360Controller::getTrigger(Trigger trigger) {
    return triggers_[trigger];
}

void XBOX360Controller::setCallBack(Button button, ButtonState state, void (*function)(void* ), void * pointer) {
    onButton[state][button] = function;
    onButtonPointers[state][button] = pointer;
}

void XBOX360Controller::setCallBack(Analog analog, ButtonState state, void (*function)(void *, core::vector2df), void * pointer) {
    onAnalog[state][analog] = function;
    onAnalogPointers[state][analog] = pointer;
}

void XBOX360Controller::setCallBack(Trigger trigger, ButtonState state, void (*function)(void *, float), void * pointer) {
    onTrigger[state][trigger] = function;
    onTriggerPointers[state][trigger] = pointer;
}

void XBOX360Controller::listenButtons() {
    for (int button = 0; button < BUTTON_COUNT; button++)
        if (GetJoystickState().IsButtonPressed(button)) {
            printf("%i: %p %p\n", button, onButton[PRESSED][button],onButtonPointers[PRESSED][button]);
            if (onButton[PRESSED][button] != NULL)
                (*onButton[PRESSED][button])(onButtonPointers[PRESSED][button]);

            buttonWasReleased[button] = true;
        }
        else
            if (buttonWasReleased[button]) {

                if (onButton[RELEASED][button] != NULL)
                    (*onButton[RELEASED][button])(onButtonPointers[RELEASED][button]);

                buttonWasReleased[button] = false;
            }
}

void XBOX360Controller::listenAnalogs() {

    float x = fixInput(GetJoystickState().Axis[L_ANALOG]);
    float y = fixInput(GetJoystickState().Axis[L_ANALOG + 1]);

    for (int analog = 0; analog < ANALOG_COUNT; analog++) {
        if (analogsMoved()) {

            if (onAnalog[PRESSED][analog] != NULL)
                (*onAnalog[PRESSED][analog])(onAnalogPointers[PRESSED][analog], getAnalog(analog));

            analogWasReleased[analog] = true;
        }
        else
            if (analogWasReleased[analog]) {

                if (onAnalog[RELEASED][analog] != NULL)
                    (*onAnalog[RELEASED][analog])(onAnalogPointers[RELEASED][analog], getAnalog(analog));

                analogWasReleased[analog] = false;
            }
    }
}

void XBOX360Controller::listenTriggers() {}

bool XBOX360Controller::analogsMoved() {

    // TODO

    return 0;
}

float XBOX360Controller::fixInput(float x) {
    return (x < getDeadZone()) ? 0 : x;
}

bool XBOX360Controller::OnEvent(const SEvent& event) {
    if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
        && event.JoystickEvent.Joystick == 0) {

        joystickState_ = event.JoystickEvent;
        listenButtons();
        listenAnalogs();
        listenTriggers();

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
