#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "SDL.h"


class FPSCounter
{
    public:

        FPSCounter();
        ~FPSCounter();

        void Start();
        void Update();

        inline Uint8 GetFPS() { return mFPS; };

    private:

        Uint8 mFPS;
        Uint8 mFPSCounter;
        Uint16 mFPSTickCounter;
        Uint32 mLastTick;
        Uint32 mTimeSpent;
};

#endif // FPSCOUNTER_H
