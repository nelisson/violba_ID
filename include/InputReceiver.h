#ifndef INPUTRECEIVER_H
#define INPUTRECEIVER_H
#include <irrlicht.h>

using namespace irr;
using namespace scene;

class InputReceiver : public IEventReceiver {
    private:
        SEvent::SJoystickEvent JoystickState;

    public:
        virtual bool OnEvent(const SEvent& event);
        const SEvent::SJoystickEvent& GetJoystickState(void) const;

        InputReceiver();
        ~InputReceiver();
};


#endif // INPUTRECEIVER_H
