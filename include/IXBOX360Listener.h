#ifndef IXBOX360LISTENER_H
#define IXBOX360LISTENER_H

#include <cstdlib>

#define BUTTON_COUNT 14
#define AXIS_COUNT   4
#define STATE_COUNT  2

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
    LT,
    RT,
    L_ANALOG,
    R_ANALOG,
};

class IXBOX360Listener {
    private:
        static bool buttonWasReleased[BUTTON_COUNT];
        void (*onButtonPressed[BUTTON_COUNT]);
        void (*onButtonReleased[BUTTON_COUNT]);

        static bool axisWasReleased[AXIS_COUNT];
        void (*onAxisMoved[AXIS_COUNT]);
        void (*onAxisReleased[AXIS_COUNT]);

    protected:

    public:
        void setCallBack(Button button, ButtonState state, void (*function));
        void setCallBack(Axis axis, ButtonState state, void (*function));

        void listenButtons();
        void listenAxis();

        IXBOX360Listener();
        virtual ~IXBOX360Listener();
};

#endif // IJOYSTICKLISTENER_H
