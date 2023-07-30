#ifndef SONGSTATISTICS_H
#define SONGSTATISTICS_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "TextManager.h"
#include "InputHandler.h"


class Game;


class SongStatistics
{
    public:

        SongStatistics(Uint16 numEvents, SDL_Surface* surface, SDL_Surface* statisticsBackground, SDL_Surface* statisticsBars, TextManager* textManager, InputHandler* inputHandler, Game* game);
        ~SongStatistics();

        inline void AddPerfect() { mNumPerfect++; };
        inline void AddGreat() { mNumGreat++; };
        inline void AddGood() { mNumGood++; };
        inline void AddPoor() { mNumPoor++; };
        inline void AddMissed() { mNumMissed++; };
        inline void SetCombo(Uint16 numCombo) { mNumCombo=numCombo; };

        void Update();
        void Draw();
        void DrawPerfect();
        void DrawGreat();
        void DrawGood();
        void DrawPoor();
        void DrawMissed();
        void DrawCombo();

        enum State {INIT,
                    START_WAITING,
                    UPDATING_PERFECT,
                    UPDATING_GREAT,
                    UPDATING_GOOD,
                    UPDATING_POOR,
                    UPDATING_MISSED,
                    UPDATING_COMBO,
                    END_WAITING,
                    DONE};
        inline State GetState() { return mState; };

    private:

        Uint16 mNumEvents;

        Uint16 mNumPerfect;
        Uint16 mNumGreat;
        Uint16 mNumGood;
        Uint16 mNumPoor;
        Uint16 mNumMissed;
        Uint16 mNumCombo;

        Uint8 mCurrentPerfect;
        Uint8 mCurrentGreat;
        Uint8 mCurrentGood;
        Uint8 mCurrentPoor;
        Uint8 mCurrentMissed;
        Uint16 mCurrentCombo;

        Uint8 mTargetPerfect;
        Uint8 mTargetGreat;
        Uint8 mTargetGood;
        Uint8 mTargetPoor;
        Uint8 mTargetMissed;

        Uint32 mMilliseconds;

        SDL_Surface* mSurface;
        SDL_Surface* mStatisticsBackground;
        SDL_Surface* mStatisticsBars;
        TextManager* mTextManager;

        State mState;

        Uint8 mUpdateSpeed;
        InputHandler* mInputHandler;

        Game* mGame;
};

#endif // SONGSTATISTICS_H
