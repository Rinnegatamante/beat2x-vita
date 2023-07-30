#ifndef GAME_H
#define GAME_H

#include "Consts.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "TextManager.h"
#include "InputHandler.h"
#include "Song.h"
#include "SongStatistics.h"
#include "ThemeManager.h"


class Game
{
    public:

        Game(char* pack, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, InputHandler* inputHandler, Uint8 speed, Uint8 controlMode, Uint8 gameMode, Uint8 initialLevel);
        virtual ~Game();

        void Update();
        void UpdateInitLevel();
        void UpdateCountdown1();
        void UpdateCountdown2();
        void UpdateCountdown3();
        void UpdateLevel();
        void UpdateStatistics();
        void UpdateEndLevel();
        void UpdateResults();
        void UpdateInitEndGame();
        void UpdateEndGame();

        void Draw();
        void DrawLevel();
        void DrawStatistics();

        void AddScore(Uint16 partialScore) { mScore += partialScore; };

        inline Uint16 GetScore() { return mScore; };
        inline Uint8 GetLevel() { return mLevel+1; };

        enum State {INIT_LEVEL,
                    COUNTDOWN_1,
                    COUNTDOWN_2,
                    COUNTDOWN_3,
                    LEVEL,
                    STATISTICS,
                    END_LEVEL,
                    RESULTS,
                    INIT_END_GAME,
                    END_GAME,
                    DEAD};
        inline State GetState() { return mState; };

    private:

        Uint16 mScore, mScoreTmp;
        Sint32 mEnergy, mEnergyTmp;
        Uint8 mLevel;
        Uint8 mSpeed; // 0=normal, 1=fast, 2=faster
        Uint16 mCombo, mLevelMaxCombo, mMaxCombo;
        bool mStopCounting;

        State mState;

        Song* mCurrentSong;
        SongStatistics* mSongStatistics;

        char* mSongList[MAX_SONGS];
        Uint8 mNumSongs;
        char* mPack;

        Uint8 mGameMode;
        Uint8 mControlMode;
        bool mKeysEnabled;

        SDL_Surface* mSurface;
        ThemeManager* mThemeManager;

        TextManager* mTextManager;
        InputHandler* mInputHandler;

        Uint32 mCountdown, mTimeShowMessage, mTimeResults, mTimerFailed;

        SDL_Rect mEnergyBarPos, mREnergyBarPos, mEnergyBarRect, mREnergyBarRect;
        SDL_Rect mScorePos, mScoreRect, mLevelPos, mLevelRect;
        SDL_Rect mMessagesPos, mMessagesRect;
        SDL_Rect mComboPos1, mComboPos2;
        SDL_Rect mComboNumbersRect;

        bool mMessageFirstFrame;
        Sint16 mMessageDisplX, mMessageDisplY;
        Sint16 mComboDisplX, mComboDisplY;
};

#endif // GAME_H
