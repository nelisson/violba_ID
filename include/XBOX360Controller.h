#ifndef XBOX360Controller_H
#define XBOX360Controller_H

#include <cstdlib>
#include <irrlicht/irrlicht.h>

#define BUTTON_COUNT 14
#define AXIS_COUNT   5
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
        float axis[AXIS_COUNT];

        float lAnalogX;
        float lAnalogY;

        float lTrigger;
        float rTrigger;

        float deadZone_;
        float fixInput(float x);

        static bool buttonWasReleased[BUTTON_COUNT];
        void (*onButton[STATE_COUNT][BUTTON_COUNT])(void *);
        void * onButtonPointers[STATE_COUNT][BUTTON_COUNT];

        static bool axisWasReleased[AXIS_COUNT];
        void (*onAxis[STATE_COUNT][AXIS_COUNT])(void * classPointer, core::vector2df direction);
        void * onAxisPointers[STATE_COUNT][AXIS_COUNT];

    protected:

    public:
        float getDeadZone();
        void setDeadZone(float deadZone);

        core::vector2df getLAnalog();

        float getLTrigger();
        float getRTrigger();

        bool anyAxisMoved();

        void setCallBack(Button button, ButtonState state, void (*function)(void *), void * pointer);
        void setCallBack(Axis axis, ButtonState state, void (*function)(void *, core::vector2df), void * pointer);

        void listenButtons();
        void listenAxis();

        virtual bool OnEvent(const SEvent& event);

        const SEvent::SJoystickEvent& GetJoystickState() const;

        XBOX360Controller(float deadZone = DEFAULT_DEADZONE);
        virtual ~XBOX360Controller();
};

#endif
