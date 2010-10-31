#include "InputReceiver.h"

bool InputReceiver::OnEvent(const SEvent& event) {

    if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
        && event.JoystickEvent.Joystick == 0)
        JoystickState = event.JoystickEvent;

    return false;
}

const SEvent::SJoystickEvent& InputReceiver::GetJoystickState(void) const {
    return JoystickState;
}

InputReceiver::InputReceiver() {
    //ctor
}

InputReceiver::~InputReceiver() {
    //dtor
}
