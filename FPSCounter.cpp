#include "FPSCounter.h"


FPSCounter::FPSCounter()
{
    mFPS = 0;
    mFPSCounter = 0;
    mFPSTickCounter = 0;
    mLastTick = 0;
}


FPSCounter::~FPSCounter()
{
}


void FPSCounter::Start()
{
    mLastTick = SDL_GetTicks();
}


void FPSCounter::Update()
{
    mTimeSpent = SDL_GetTicks() - mLastTick;

    mLastTick = SDL_GetTicks();
    mFPSTickCounter += mTimeSpent;

    ++mFPSCounter;
    if (mFPSTickCounter >= 1000)
    {
        mFPS = mFPSCounter;
        mFPSCounter = 0;
        mFPSTickCounter = 0;
    }
}
