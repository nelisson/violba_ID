#ifndef XBOX360Controller_H
#define XBOX360Controller_H

#include <cstdlib>
#include <irrlicht/irrlicht.h>

#define BUTTON_COUNT 14
#define AXIS_COUNT   4
#define STATE_COUNT  2

#define DEFAULT_DEADZONE 0.2f

using namespace irr;

enum Button {
    A,
    B,
    X,
    Y,
    L,
    R,
    START,
    SELECT,
    POV_UP,
    POV_DOWN,
    POV_LEFT,
    POV_RIGHT,
    L_ANALOG_BUTTON,
    R_ANALOG_BUTTON,
};

enum ButtonState {
    PRESSED,
    RELEASED,
};

enum Axis {
    LT = SEvent::SJoystickEvent::AXIS_Z,
    RT = SEvent::SJoystickEvent::AXIS_Z,
    L_ANALOG = SEvent::SJoystickEvent::AXIS_X,
    R_ANALOG = SEvent::SJoystickEvent::AXIS_U,
};

class XBOX360Controller : public IEventReceiver {

    private:
        SEvent::SJoystickEvent joystickState_;
        float lAnalogX
        float lAnalogY

        float deadZone_;
        float fixInput(float x);

        static bool buttonWasReleased[BUTTON_COUNT];
        void (*onButtonPressed[BUTTON_COUNT])(void *);
        void * onButtonPressedPointers[BUTTON_COUNT];
        void (*onButtonReleased[BUTTON_COUNT])(void *);
        void * onButtonReleasedPointers[BUTTON_COUNT];

        static bool axisWasReleased[AXIS_COUNT];
        void (*onAxisMoved[AXIS_COUNT])(void * classPointer, core::vector2d direction);
        void * onAxisMovedPointers[AXIS_COUNT];
        void (*onAxisReleased[AXIS_COUNT])(void * classPointer, core::vector2d direction);
        void * onAxisReleasedPointers[AXIS_COUNT];

    protected:

    public:
        float getDeadZone();
        void setDeadZone(float deadZone);

        void setCallBack(Button button, ButtonState state, void (*function)(void *), void * pointer);
        void setCallBack(Axis axis, ButtonState state, void (*function)(void *), void * pointer);

        void listenButtons();
        void listenAxis();

        virtual bool OnEvent(const SEvent& event);

        const SEvent::SJoystickEvent& GetJoystickState() const;

        XBOX360Controller(float deadZone = DEFAULT_DEADZONE);
        virtual ~XBOX360Controller();
};

#endif
