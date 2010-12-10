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

core::vector2df XBOX360Controller::getAnalog(int analog) {
    return analogs_[analog];
}

float XBOX360Controller::getTrigger(int trigger) {
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
    for (int analog = 0; analog < ANALOG_COUNT; analog++) {
        analogs_[analog] = core::vector2df(fixInput(GetJoystickState().Axis[toAxis(analog)]),
                                           fixInput(GetJoystickState().Axis[toAxis(analog) + 1]));

        if (analogMoved(analog)) {

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

void XBOX360Controller::listenTriggers() {
    float value = fixInput(GetJoystickState().Axis[SEvent::SJoystickEvent::AXIS_Z]);
    int trigger = (value > 0) ? LT : RT;
    triggers_[trigger] = value;

    if (triggersMoved()) {

        if (onTrigger[PRESSED][trigger] != NULL)
            (*onTrigger[PRESSED][trigger])(onTriggerPointers[PRESSED][trigger], getTrigger(trigger));

        triggerWasReleased[trigger] = true;
    }
    else
        if (triggerWasReleased[trigger]) {

            if (onTrigger[RELEASED][trigger] != NULL)
                (*onTrigger[RELEASED][trigger])(onTriggerPointers[RELEASED][trigger], getTrigger(trigger));

            triggerWasReleased[trigger] = false;
        }
}

bool XBOX360Controller::analogMoved(int analog) {
    return analogs_[analog].X || analogs_[analog].Y;
}

bool XBOX360Controller::triggersMoved() {
    return triggers_[LT] || triggers_[RT];
}

float XBOX360Controller::fixInput(float x) {
    return (abs(x) < getDeadZone()) ? 0 : x;
}

int XBOX360Controller::toAxis(int analog) {
    switch(analog) {
        case L_ANALOG : return SEvent::SJoystickEvent::AXIS_X;
        case R_ANALOG : return SEvent::SJoystickEvent::AXIS_U;
        default : return NULL;
    }
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
    if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			//IGUIEnvironment* env = device_->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
                                    printf("QUITclicked\n");
					device_->closeDevice();
					return true;

				case GUI_ID_PLAY_DEMO_BUTTON:
					{
                                            *mainScreen = false;
					}
					return true;

				default:
					return false;
				}
				break;

			default:
				break;
			}
		}

    else
        return false;
}

const SEvent::SJoystickEvent& XBOX360Controller::GetJoystickState(void) const {
    return joystickState_;
}

XBOX360Controller::XBOX360Controller(float deadZone) {
    deadZone_ = deadZone;
    for (int state = 0; state < STATE_COUNT; state++) {
        for (int button = 0; button < BUTTON_COUNT; button++)
            onButton[state][button] = NULL;

        for (int analog = 0; analog < ANALOG_COUNT; analog++)
            onAnalog[state][analog] = NULL;

        for (int trigger = 0; trigger < TRIGGER_COUNT; trigger++)
            onTrigger[state][trigger] = NULL;
    }
}

XBOX360Controller::~XBOX360Controller() {}
