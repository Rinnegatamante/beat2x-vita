#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "InputHandler.h"
#include "FPSCounter.h"
#include "TextManager.h"
#include "MenuMain.h"
#include "MenuHiscores.h"
#include "MenuChoosePack.h"
#include "MenuChooseLevel.h"
#include "MenuEnterInitials.h"
#include "MenuSettings.h"
#include "Game.h"
#include "Song.h"
#include "ScoresManager.h"
#include "ThemeManager.h"
#include "Settings.h"


class Engine
{
    public:

        Engine();
        ~Engine();

        void Init();
        void Start();
        void End();

        void Draw();

        void Update();
        void UpdateSplash();
        void UpdateInitMainMenu();
        void UpdateMainMenu();
        void UpdateInitChoosePackMenu();
        void UpdateChoosePackMenu();
        void UpdateInitChooseLevelMenu();
        void UpdateChooseLevelMenu();
        void UpdateInitSettingsMenu();
        void UpdateSettingsMenu();
        void UpdateInitScores();
        void UpdateScores();
        void UpdateInitInitials();
        void UpdateInitials();
        void UpdateInitGame();
        void UpdateGame();
        void UpdateResults();

    private:

        enum State {    INIT_MAIN_MENU,
                        MAIN_MENU,
                        INIT_CHOOSE_PACK_MENU,
                        CHOOSE_PACK_MENU,
                        INIT_CHOOSE_LEVEL_MENU,
                        CHOOSE_LEVEL_MENU,
                        INIT_SETTINGS_MENU,
                        SETTINGS_MENU,
                        INIT_SCORES,
                        SCORES,
                        INIT_INITIALS,
                        INITIALS,
                        INIT_GAME,
                        GAME,
                        QUIT };
        State mState;

        SDL_Surface* mSurface;
        SDL_Surface* mSplash;

        Settings mSettings;
        ThemeManager* mThemeManager;
        TextManager* mTextManager;

        SDL_Joystick *mJoystick;

        Sint16 mVolume;
        bool mShowVolume;
        Uint32 mTimeVolume;

        Uint32 mTimeQuitGame;

        MenuMain* mMenuMain;
        MenuHiscores* mMenuHiscores;
        MenuChoosePack* mMenuChoosePack;
        MenuEnterInitials* mMenuEnterInitials;
        MenuChooseLevel* mMenuChooseLevel;
        MenuSettings* mMenuSettings;

        Game* mGame;
        Uint8 mInitialLevel;
        ScoresManager mScoresManager;

        char mCurrentPackFolder[128];
        Uint8 mCurrentPack;
        Uint8 mLastPlayedSong;

        InputHandler mInputHandler;
        Uint8 mKeyDelay, mKeyInitDelay;

        FPSCounter mFPSCounter;

        bool mQuit;

        Uint32 mMilliseconds;
};

#endif // ENGINE_H
