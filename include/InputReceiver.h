#ifndef INPUTRECEIVER_H
#define INPUTRECEIVER_H

#include <irrlicht.h>
#include "IXBOX360Listener.h"

using namespace irr;
using namespace scene;

class InputReceiver : public IEventReceiver, public IXBOX360Listener {
    private:
        SEvent::SJoystickEvent JoystickState;

    protected:

    public:
        virtual bool OnEvent(const SEvent& event);

        const SEvent::SJoystickEvent& GetJoystickState(void) const;

        InputReceiver();
        ~InputReceiver();
};


#endif // INPUTRECEIVER_H
