#include "ThemeManager.h"
#ifdef TARGET_WIN32
#   include <windows.h>
#endif
#include <iostream>

#ifndef TARGET_WIN32
extern int alphasort();
#include <dirent.h>
#endif

#include "SDL_image.h"


ThemeManager::ThemeManager(SDL_Surface* surface)
{
    mSurface = surface;

    mSoundGo=NULL;
    mSoundCountdown=NULL;
    mMusicStatistics=NULL;
    mIntro=NULL;
    mCharSelector=NULL;
    mFont=NULL;
    mFailed=NULL;
    mCleared=NULL;
    mStatisticsBars=NULL;
    mComboNumbers=NULL;
    mCombo=NULL;
    mMessages=NULL;
    mBoardText=NULL;
    mEnergyBar=NULL;
    mButtonSet=NULL;
    mStatisticsBackground=NULL;
    mGameBackground=NULL;
    mMainMenuBackground=NULL;
    mMenuBackground=NULL;

    // Read themes folders

    Uint8 numThemes=0;

    mThemeNames.push_back(new char[8]);
    strcpy(mThemeNames[numThemes], "default");
    numThemes++;

#ifndef TARGET_WIN32
    struct dirent **list;
    int n=scandir("themes", &list, NULL, alphasort);

    for (Uint8 i=0; i<n; i++)
    {
        if ((strcmp(list[i]->d_name, ".")!=0) && (strcmp(list[i]->d_name, "..")!=0) && (strcmp(list[i]->d_name, "default")!=0))
        {
            mThemeNames.push_back(new char[strlen(list[i]->d_name)+1]);
            strcpy(mThemeNames[numThemes], list[i]->d_name);
            numThemes++;
        }
        free(list[i]);
    }
#else
    WIN32_FIND_DATA data;
    HANDLE folders = FindFirstFile("themes/*", &data);

    if (folders==INVALID_HANDLE_VALUE)
    {
        cerr << "Error reading <packs>" << endl;
        exit(1);
    }
    else
    {
        if ((strcmp(data.cFileName, ".")!=0) && (strcmp(data.cFileName, "..")!=0) && (strcmp(data.cFileName, "default")!=0))
        {
            mThemeNames.push_back(new char[strlen(data.cFileName)+1]);
            strcpy(mThemeNames[numThemes], data.cFileName);
            numThemes++;
        }
        while (FindNextFile(folders, &data)!=0)
        {
            if ((strcmp(data.cFileName, ".")!=0) && (strcmp(data.cFileName, "..")!=0) && (strcmp(data.cFileName, "default")!=0))
            {
                mThemeNames.push_back(new char[strlen(data.cFileName)+1]);
                strcpy(mThemeNames[numThemes], data.cFileName);
                numThemes++;
            }
        }
        FindClose(folders);
    }
#endif

    SDL_Surface* tmpLoading = IMG_Load("data/loading.bmp");
    if (tmpLoading == NULL)
    {
        cerr << "Error reading <data/loading.bmp>: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_SetColorKey(tmpLoading, SDL_SRCCOLORKEY, SDL_MapRGB(mSurface->format, 255, 0, 255));
    mLoading = SDL_DisplayFormat(tmpLoading);
    SDL_FreeSurface(tmpLoading);

    mCurrentTheme=0;
    mNumFiles = 19;
}


ThemeManager::~ThemeManager()
{
    for (Uint8 i=0; i<mThemeNames.size(); i++) delete mThemeNames[i];
        mThemeNames.clear();

    FreeResources();

    if (mLoading!=NULL) SDL_FreeSurface(mLoading);
}


void ThemeManager::SetTheme(Uint8 idTheme)
{
    if (idTheme<mThemeNames.size())
        mCurrentTheme = idTheme;
    else
        mCurrentTheme = 0;

    ReadTheme(mThemeNames[mCurrentTheme]);
}


void ThemeManager::NextTheme()
{
    mCurrentTheme=(mCurrentTheme+1)%mThemeNames.size();
    SetTheme(mCurrentTheme);
}


void ThemeManager::ReadTheme(char* themeFolder)
{
    FreeResources();

    mLoadedFiles=0;

    ReadFile(&mMainMenuBackground, themeFolder, "mainmenubackground");
    ReadFile(&mMenuBackground, themeFolder, "menubackground");
    ReadFile(&mButtonSet, themeFolder, "buttonset");
    ReadFile(&mEnergyBar, themeFolder, "energybar");
    ReadFile(&mMessages, themeFolder, "messages");
    ReadFile(&mCombo, themeFolder, "combo");
    ReadFile(&mComboNumbers, themeFolder, "combonumbers");
    ReadFile(&mBoardText, themeFolder, "boardtext");
    ReadFile(&mGameBackground, themeFolder, "gamebackground");
    ReadFile(&mStatisticsBackground, themeFolder, "statisticsbackground");
    ReadFile(&mCleared, themeFolder, "cleared");
    ReadFile(&mFailed, themeFolder, "failed");
    ReadFile(&mStatisticsBars, themeFolder, "statisticsbars");
    ReadFile(&mFont, themeFolder, "font");
    ReadFile(&mCharSelector, themeFolder, "charselector");

    ReadFile(&mIntro, themeFolder, "intro");
    ReadFile(&mMusicStatistics, themeFolder, "statistics");

    ReadFile(&mSoundCountdown, themeFolder, "countdown");
    ReadFile(&mSoundGo, themeFolder, "go");
}


void ThemeManager::ReadFile(SDL_Surface** dest, char* path, char* filename)
{
    char fullname[256];
    sprintf(fullname, "themes/%s/%s.bmp", path, filename);

    SDL_Surface* tmp = IMG_Load(fullname);
    if (tmp == NULL)
    {
        char defaultname[256];
        sprintf(defaultname, "themes/default/%s.bmp", filename);
        tmp = IMG_Load(defaultname);
        if (tmp == NULL)
        {
            cerr << "Error reading <" << defaultname << ">: " << SDL_GetError() << endl;
            exit(1);
        }
    }
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(mSurface->format, 255, 0, 255));
    *dest = SDL_DisplayFormat(tmp);
    SDL_FreeSurface(tmp);

    DrawBar();
}


void ThemeManager::ReadFile(Mix_Music** dest, char* path, char* filename)
{
    char fullname[256];
    sprintf(fullname, "themes/%s/%s.ogg", path, filename);
    *dest = Mix_LoadMUS(fullname);
    if (*dest==NULL)
    {
        char defaultname[256];
        sprintf(defaultname, "themes/default/%s.ogg", filename);
        *dest = Mix_LoadMUS(defaultname);
        if (*dest == NULL)
        {
            cerr << "Error reading <" << defaultname << ">: " << Mix_GetError() << endl;
            exit(1);
        }
    }

    DrawBar();
}


void ThemeManager::ReadFile(Mix_Chunk** dest, char* path, char* filename)
{
    char fullname[256];
    sprintf(fullname, "themes/%s/%s.ogg", path, filename);

    *dest = Mix_LoadWAV(fullname);
    if (*dest==NULL)
    {
        char defaultname[256];
        sprintf(defaultname, "themes/default/%s.ogg", filename);
        *dest = Mix_LoadWAV(defaultname);
        if (*dest == NULL)
        {
            cerr << "Error reading <" << defaultname << ">: " << Mix_GetError() << endl;
            exit(1);
        }
    }

    DrawBar();
}


void ThemeManager::FreeResources()
{
    if (mSoundGo!=NULL) Mix_FreeChunk(mSoundGo);
    if (mSoundCountdown!=NULL) Mix_FreeChunk(mSoundCountdown);
    if (mMusicStatistics!=NULL) Mix_FreeMusic(mMusicStatistics);
    if (mIntro!=NULL) Mix_FreeMusic(mIntro);
    if (mCharSelector!=NULL) SDL_FreeSurface(mCharSelector);
    if (mFont!=NULL) SDL_FreeSurface(mFont);
    if (mFailed!=NULL) SDL_FreeSurface(mFailed);
    if (mCleared!=NULL) SDL_FreeSurface(mCleared);
    if (mStatisticsBars!=NULL) SDL_FreeSurface(mStatisticsBars);
    if (mComboNumbers!=NULL) SDL_FreeSurface(mComboNumbers);
    if (mCombo!=NULL) SDL_FreeSurface(mCombo);
    if (mMessages!=NULL) SDL_FreeSurface(mMessages);
    if (mBoardText!=NULL) SDL_FreeSurface(mBoardText);
    if (mEnergyBar!=NULL) SDL_FreeSurface(mEnergyBar);
    if (mButtonSet!=NULL) SDL_FreeSurface(mButtonSet);
    if (mStatisticsBackground!=NULL) SDL_FreeSurface(mStatisticsBackground);
    if (mGameBackground!=NULL) SDL_FreeSurface(mGameBackground);
    if (mMainMenuBackground!=NULL) SDL_FreeSurface(mMainMenuBackground);
    if (mMenuBackground!=NULL) SDL_FreeSurface(mMenuBackground);
    mSoundGo=NULL;
    mSoundCountdown=NULL;
    mMusicStatistics=NULL;
    mIntro=NULL;
    mCharSelector=NULL;
    mFont=NULL;
    mFailed=NULL;
    mCleared=NULL;
    mStatisticsBars=NULL;
    mComboNumbers=NULL;
    mCombo=NULL;
    mMessages=NULL;
    mBoardText=NULL;
    mEnergyBar=NULL;
    mButtonSet=NULL;
    mStatisticsBackground=NULL;
    mGameBackground=NULL;
    mMainMenuBackground=NULL;
    mMenuBackground=NULL;
}


void ThemeManager::DrawBar()
{
    mLoadedFiles++;

    SDL_Rect barRect, barPos;
    barRect.x=0;
    barRect.y=0;
    barRect.h=43;
    barRect.w=mLoadedFiles*320/mNumFiles;
    barPos.x=0;
    barPos.y=98;
    SDL_BlitSurface(mLoading, &barRect, mSurface, &barPos);
    SDL_Flip(mSurface);
}
