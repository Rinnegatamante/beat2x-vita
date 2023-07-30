#include "SongStatistics.h"

#include "Consts.h"

#include "Game.h"


SongStatistics::SongStatistics(Uint16 numEvents, SDL_Surface* surface, SDL_Surface* statisticsBackground, SDL_Surface* statisticsBars, TextManager* textManager, InputHandler* inputHandler, Game* game)
{
    mNumEvents=numEvents;

    mNumPerfect=0;
    mNumGreat=0;
    mNumGood=0;
    mNumCombo=0;
    mNumPoor=0;
    mNumMissed=0;

    mCurrentPerfect=0;
    mCurrentGreat=0;
    mCurrentGood=0;
    mCurrentCombo=0;
    mCurrentPoor=0;
    mCurrentMissed=0;

    mSurface=surface;
    mStatisticsBackground = statisticsBackground;
    mStatisticsBars = statisticsBars;
    mTextManager=textManager;
    mInputHandler = inputHandler;

    mGame=game;

    mUpdateSpeed=1;

    mState=INIT;
}

SongStatistics::~SongStatistics()
{
}


void SongStatistics::Update()
{
    if (mInputHandler->IsA()||mInputHandler->IsB()||mInputHandler->IsX()||mInputHandler->IsY()||mInputHandler->IsL()||mInputHandler->IsR()||mInputHandler->IsStart()||mInputHandler->IsSelect())
    {
        switch(mState)
        {
            case START_WAITING: mState=UPDATING_PERFECT; break;
            case UPDATING_PERFECT:
            case UPDATING_GREAT:
            case UPDATING_GOOD:
            case UPDATING_POOR:
            case UPDATING_MISSED:
            case UPDATING_COMBO: mUpdateSpeed=5; break;
            case END_WAITING: mState=DONE;
            default: break;
        }
    }

    switch(mState)
    {
        case INIT:
        {
            mMilliseconds = SDL_GetTicks();
            mState = START_WAITING;
            break;
        }
        case START_WAITING:
        {
            mTargetPerfect = (mNumPerfect*100)/mNumEvents;
            mTargetGreat = (mNumGreat*100)/mNumEvents;
            mTargetGood = (mNumGood*100)/mNumEvents;
            mTargetPoor = (mNumPoor*100)/mNumEvents;
            mTargetMissed = (mNumMissed*100)/mNumEvents;

            if (SDL_GetTicks()-mMilliseconds>1000) mState=UPDATING_PERFECT;
            break;
        }
        case UPDATING_PERFECT:
        {
            if (mCurrentPerfect<mTargetPerfect)
                mCurrentPerfect+=mUpdateSpeed;
            else
                mState=UPDATING_GREAT;
            break;
        }
        case UPDATING_GREAT:
        {
            if (mCurrentGreat<mTargetGreat)
                mCurrentGreat+=mUpdateSpeed;
            else
                mState=UPDATING_GOOD;
            break;
        }
        case UPDATING_GOOD:
        {
            if (mCurrentGood<mTargetGood)
                mCurrentGood+=mUpdateSpeed;
            else
                mState=UPDATING_POOR;
            break;
        }
        case UPDATING_POOR:
        {
            if (mCurrentPoor<mTargetPoor)
                mCurrentPoor+=mUpdateSpeed;
            else
                mState=UPDATING_MISSED;
            break;
        }
        case UPDATING_MISSED:
        {
            if (mCurrentMissed<mTargetMissed)
                mCurrentMissed+=mUpdateSpeed;
            else
                mState=UPDATING_COMBO;
            break;
        }
        case UPDATING_COMBO:
        {
            if (mCurrentCombo<mNumCombo)
            {
                Uint16 add=mUpdateSpeed*2;
                if (mCurrentCombo+add>mNumCombo) add=mNumCombo-mCurrentCombo;

                mCurrentCombo+=add;
                mGame->AddScore(add*5);
            }
            else
            {
                mMilliseconds = SDL_GetTicks();
                mState=END_WAITING;
            }
            break;
        }
        case END_WAITING:
        {
            if (SDL_GetTicks()-mMilliseconds>3000) mState=DONE;

            break;
        }
        default: break;
    }
}


void SongStatistics::Draw()
{
    SDL_BlitSurface(mStatisticsBackground, NULL, mSurface, NULL);

    switch(mState)
    {
        case UPDATING_PERFECT:  DrawPerfect(); break;
        case UPDATING_GREAT:    DrawPerfect(); DrawGreat(); break;
        case UPDATING_GOOD:     DrawPerfect(); DrawGreat(); DrawGood(); break;
        case UPDATING_POOR:     DrawPerfect(); DrawGreat(); DrawGood(); DrawPoor(); break;
        case UPDATING_MISSED:   DrawPerfect(); DrawGreat(); DrawGood(); DrawPoor(); DrawMissed(); break;
        case UPDATING_COMBO:
        case END_WAITING:       DrawPerfect(); DrawGreat(); DrawGood(); DrawPoor(); DrawMissed(); DrawCombo(); break;
        default: break;
    }
}


void SongStatistics::DrawPerfect()
{
    static char perfect[5];
    static SDL_Rect rPerfect, pPerfect;
    Uint16 xInit=110, yInit=67;

    mTextManager->Write(34,yInit,"  PERFECT");

    pPerfect.x=xInit; pPerfect.y=yInit-2;
    rPerfect.x=0; rPerfect.y=0;
    if (mCurrentPerfect>0)
        rPerfect.w=(mCurrentPerfect*(RESX-pPerfect.x-5*CHAR_SIZE))/100;
    else
        rPerfect.w=1;
    rPerfect.h=12;

    sprintf(perfect, "%d%%", mCurrentPerfect);
    mTextManager->Write(pPerfect.x+rPerfect.w+4+CHAR_SIZE*2,yInit,perfect,HCENTRE);

    SDL_BlitSurface(mStatisticsBars, &rPerfect, mSurface, &pPerfect);
}


void SongStatistics::DrawGreat()
{
    static char great[5];
    static SDL_Rect rGreat, pGreat;
    Uint16 xInit=110, yInit=67;

    mTextManager->Write(34,yInit+20,"    GREAT");

    pGreat.x=xInit; pGreat.y=yInit+18;
    rGreat.x=0; rGreat.y=12;
    if (mCurrentGreat>0)
        rGreat.w=(mCurrentGreat*(RESX-pGreat.x-5*CHAR_SIZE))/100;
    else
        rGreat.w=1;
    rGreat.h=12;

    sprintf(great, "%d%%", mCurrentGreat);
    mTextManager->Write(pGreat.x+rGreat.w+4+CHAR_SIZE*2,yInit+20,great,HCENTRE);

    SDL_BlitSurface(mStatisticsBars, &rGreat, mSurface, &pGreat);
}


void SongStatistics::DrawGood()
{
    static char good[5];
    static SDL_Rect rGood, pGood;
    Uint16 xInit=110, yInit=67;

    mTextManager->Write(34,yInit+40,"     GOOD");

    pGood.x=xInit; pGood.y=yInit+38;
    rGood.x=0; rGood.y=24;
    if (mCurrentGood>0)
        rGood.w=(mCurrentGood*(RESX-pGood.x-5*CHAR_SIZE))/100;
    else
        rGood.w=1;
    rGood.h=12;

    sprintf(good, "%d%%", mCurrentGood);
    mTextManager->Write(pGood.x+rGood.w+4+CHAR_SIZE*2,yInit+40,good,HCENTRE);

    SDL_BlitSurface(mStatisticsBars, &rGood, mSurface, &pGood);
}


void SongStatistics::DrawPoor()
{
    static char poor[5];
    static SDL_Rect rPoor, pPoor;
    Uint16 xInit=110, yInit=67;

    mTextManager->Write(34,yInit+60,"     POOR");

    pPoor.x=xInit; pPoor.y=yInit+58;
    rPoor.x=0; rPoor.y=36;
    if (mCurrentPoor>0)
        rPoor.w=(mCurrentPoor*(RESX-pPoor.x-5*CHAR_SIZE))/100;
    else
        rPoor.w=1;
    rPoor.h=12;

    sprintf(poor, "%d%%", mCurrentPoor);
    mTextManager->Write(pPoor.x+rPoor.w+4+CHAR_SIZE*2,yInit+60,poor,HCENTRE);

    SDL_BlitSurface(mStatisticsBars, &rPoor, mSurface, &pPoor);
}


void SongStatistics::DrawMissed()
{
    static char missed[5];
    static SDL_Rect rMissed, pMissed;
    Uint16 xInit=110, yInit=67;

    mTextManager->Write(34,yInit+80,"   MISSED");

    pMissed.x=xInit; pMissed.y=yInit+78;
    rMissed.x=0; rMissed.y=36;
    if (mCurrentMissed>0)
        rMissed.w=(mCurrentMissed*(RESX-pMissed.x-5*CHAR_SIZE))/100;
    else
        rMissed.w=1;
    rMissed.h=12;

    sprintf(missed, "%d%%", mCurrentMissed);
    mTextManager->Write(pMissed.x+rMissed.w+4+CHAR_SIZE*2,yInit+80,missed,HCENTRE);

    SDL_BlitSurface(mStatisticsBars, &rMissed, mSurface, &pMissed);
}


void SongStatistics::DrawCombo()
{
    static char combo[10];
    Uint16 xInit=110, yInit=67;

    mTextManager->Write(34,yInit+100,"MAX COMBO");

    sprintf(combo, "%d", mCurrentCombo);
    mTextManager->Write(xInit+CHAR_SIZE/2,yInit+100,combo);
}
