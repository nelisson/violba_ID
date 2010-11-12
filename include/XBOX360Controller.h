#ifndef XBOX360Controller_H
#define XBOX360Controller_H

#include <cstdlib>
#include <irrlicht/irrlicht.h>

#define BUTTON_COUNT 14
#define ANALOG_COUNT  5
#define TRIGGER_COUNT 2
#define STATE_COUNT   2

#define DEFAULT_DEADZONE 6000.f

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

enum Analog {
    L_ANALOG,// = SEvent::SJoystickEvent::AXIS_X,
    R_ANALOG,// = SEvent::SJoystickEvent::AXIS_U,
};

enum Trigger {
    LT,// = SEvent::SJoystickEvent::AXIS_Z,
    RT,// = SEvent::SJoystickEvent::AXIS_Z,
};

class XBOX360Controller : public IEventReceiver {

    private:
        SEvent::SJoystickEvent joystickState_;
        float triggers_[TRIGGER_COUNT];
        core::vector2df analogs_[ANALOG_COUNT];

        //bool isIdle;
        float deadZone_;
        float fixInput(float x);
        int toAxis(int analog);

        static bool buttonWasReleased[BUTTON_COUNT];
        void (*onButton[STATE_COUNT][BUTTON_COUNT])(void *);
        void * onButtonPointers[STATE_COUNT][BUTTON_COUNT];

        static bool analogWasReleased[ANALOG_COUNT];
        void (*onAnalog[STATE_COUNT][ANALOG_COUNT])(void * classPointer, core::vector2df direction);
        void * onAnalogPointers[STATE_COUNT][ANALOG_COUNT];

        static bool triggerWasReleased[TRIGGER_COUNT];
        void (*onTrigger[STATE_COUNT][TRIGGER_COUNT])(void * classPointer, float value);
        void * onTriggerPointers[STATE_COUNT][TRIGGER_COUNT];

    protected:

    public:
        float getDeadZone();
        void setDeadZone(float deadZone);

        float getTrigger(int trigger);
        core::vector2df getAnalog(int analog);

        bool analogMoved(int analog);
        bool triggersMoved();
        bool isIdle();

        void setCallBack(Button button, ButtonState state, void (*function)(void *), void * pointer);
        void setCallBack(Analog analog, ButtonState state, void (*function)(void *, core::vector2df), void * pointer);
        void setCallBack(Trigger trigger, ButtonState state, void (*function)(void *, float), void * pointer);

        void listenButtons();
        void listenAnalogs();
        void listenTriggers();

        virtual bool OnEvent(const SEvent& event);

        const SEvent::SJoystickEvent& GetJoystickState() const;

        XBOX360Controller(float deadZone = DEFAULT_DEADZONE);
        virtual ~XBOX360Controller();
};

#endif
