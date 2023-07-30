#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <vector>

using namespace std;


class ThemeManager
{
    public:

        ThemeManager(SDL_Surface* surface);
        virtual ~ThemeManager();

        void SetTheme(Uint8 idTheme);

        inline Uint8 GetNumThemes() { return mThemeNames.size(); };
        void NextTheme();

        Uint8 GetCurrentTheme() { return mCurrentTheme; };
        char* GetCurrentThemeName() { return mThemeNames[mCurrentTheme]; };

        SDL_Surface* mMainMenuBackground;
        SDL_Surface* mMenuBackground;
        SDL_Surface* mGameBackground;
        SDL_Surface* mStatisticsBackground;
        SDL_Surface* mBoardText;
        SDL_Surface* mButtonSet;
        SDL_Surface* mEnergyBar;
        SDL_Surface* mMessages;
        SDL_Surface* mCombo;
        SDL_Surface* mComboNumbers;
        SDL_Surface* mCleared;
        SDL_Surface* mFailed;
        SDL_Surface* mStatisticsBars;
        SDL_Surface* mFont;
        SDL_Surface* mCharSelector;

        Mix_Chunk *mSoundCountdown;
        Mix_Chunk *mSoundGo;

        Mix_Music *mIntro;
        Mix_Music *mMusicStatistics;

        SDL_Surface* mLoading;

    private:

        void ReadTheme(char* themeFolder);
        void ReadFile(SDL_Surface** dest, char* path, char* filename);
        void ReadFile(Mix_Music** dest, char* path, char* filename);
        void ReadFile(Mix_Chunk** dest, char* path, char* filename);

        void DrawBar();

        void FreeResources();

        SDL_Surface* mSurface;

        vector<char*> mThemeNames;
        Uint8 mCurrentTheme;

        Uint8 mNumFiles;
        Uint8 mLoadedFiles;
};

#endif // THEMEMANAGER_H
