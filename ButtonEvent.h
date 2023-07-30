#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H

#include "SDL.h"


class ButtonEvent
{
    public:

        ButtonEvent();
        ~ButtonEvent();

        enum Button { L=0, A=1, X=2, Y=3, B=4, R=5 };
        enum State { IDLE, SHOWN, RIGHT, WRONG, DISABLED, DEAD };

        void SetButton(Button button) { mButton = button; };
        void SetTimePress(Uint32 timePress) { mTimePress = timePress; };
        void SetState(State state) { mState = state; };

        inline Button GetButton() { return mButton; };
        inline Uint32 GetTimePress() { return mTimePress; };
        inline State GetState() { return mState; };

    private:

        Button mButton;
        Uint32 mTimePress;
        State mState;
};

#endif // BUTTONEVENT_H
