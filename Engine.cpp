#include "Engine.h"
#include <iostream>
#include "Consts.h"
#include "SDL_image.h"
#include "gfx.h"


using namespace std;


Engine::Engine()
{
    mQuit = false;
}


Engine::~Engine()
{
    SDL_Quit();
}


void Engine::Init()
{
    atexit(SDL_Quit);

#if defined(TARGET_GP2X) || defined(__vita__)
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) < 0)
#else
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) < 0)
#endif
    {
        cerr << "Error initializing SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    mSurface = SDL_SetVideoMode(320, 240, RESCOL, SDL_SWSURFACE | SDL_FULLSCREEN);
    if (mSurface == NULL)
    {
        cerr << "Error creating surface: " << SDL_GetError() << endl;
        exit(1);
    }

#ifdef __vita__
	SDL_VITA_SetVideoModeScaling(0, 0, 960, 544);
#endif

    SDL_ShowCursor(SDL_DISABLE);

    SDL_Surface* tmpSplash = IMG_Load("data/splash.bmp");
    if (tmpSplash == NULL)
    {
        cerr << "Error reading <data/splash.bmp>: " << SDL_GetError() << endl;
        exit(1);
    }
    SDL_SetColorKey(tmpSplash, SDL_SRCCOLORKEY, SDL_MapRGB(mSurface->format, 255, 0, 255));
    mSplash = SDL_DisplayFormat(tmpSplash);
    SDL_FreeSurface(tmpSplash);

    SDL_BlitSurface(mSplash, NULL, mSurface, NULL);
    SDL_Flip(mSurface);

#ifdef TARGET_GP2X
    if (Mix_OpenAudio(22050, AUDIO_S16, 2, 512) == -1)
#else
    if (Mix_OpenAudio(22050, AUDIO_S16, 2, 2048) == -1)
#endif
    {
        cerr << "Error opening audio device: " << Mix_GetError() << endl;
        exit(1);
    }

    mVolume=MIX_MAX_VOLUME;

    Mix_Volume(-1,mVolume);
    Mix_VolumeMusic(mVolume);

    mSettings.Read();

    mThemeManager = new ThemeManager(mSurface);
    mThemeManager->SetTheme(mSettings.mTheme);

    mTextManager = new TextManager(mThemeManager, mSurface);

#if defined(TARGET_GP2X) || defined(__vita__)
    mJoystick = SDL_JoystickOpen(0);
#else
    SDL_WM_SetCaption("Beat2X", 0);
    mJoystick = NULL;
#endif

    mMenuMain = NULL;
    mMenuChoosePack = NULL;

    mGame = NULL;
    mCurrentPack=0;
    mInitialLevel=0;

    mScoresManager.CleanUpFile();

    mKeyInitDelay = 7;
    mKeyDelay = mKeyInitDelay;

    mShowVolume=false;

    mState = INIT_MAIN_MENU;
    mMilliseconds = SDL_GetTicks();
}


void Engine::Start()
{
//    mFPSCounter.Start();

    while (!mQuit)
    {
        if (SDL_GetTicks()-mMilliseconds>=FRAME_RATE)
        {
            Update();
            Draw();

            mMilliseconds=SDL_GetTicks();
        }
    }

    End();
}


void Engine::End()
{
    if (mGame!=NULL) delete mGame;
    if (mMenuMain!=NULL) delete mMenuMain;
    Mix_CloseAudio();
    delete mTextManager;
    delete mThemeManager;

#if defined(TARGET_GP2X) || defined(__vita__)
    SDL_JoystickClose(mJoystick);
#endif
}


void Engine::Update()
{
//    mFPSCounter.Update();

    mInputHandler.Update();

    if (mKeyDelay>0) mKeyDelay--;
    if (mKeyDelay==0)
    {
        if (mInputHandler.IsVolumeUp())
        {
            mVolume+=8;
            if (mVolume>MIX_MAX_VOLUME) mVolume=MIX_MAX_VOLUME;

            Mix_Volume(-1,mVolume);
            Mix_VolumeMusic(mVolume);
            mShowVolume=true;
            mTimeVolume=mMilliseconds;

            mKeyDelay=mKeyInitDelay;
        }
        else if (mInputHandler.IsVolumeDown())
        {
            mVolume-=8;
            if (mVolume<0) mVolume=0;

            Mix_Volume(-1,mVolume);
            Mix_VolumeMusic(mVolume);
            mShowVolume=true;
            mTimeVolume=mMilliseconds;

            mKeyDelay=mKeyInitDelay;
        }
    }

    switch(mState)
    {
        case INIT_MAIN_MENU: UpdateInitMainMenu(); break;
        case MAIN_MENU: UpdateMainMenu(); break;
        case INIT_CHOOSE_PACK_MENU: UpdateInitChoosePackMenu(); break;
        case CHOOSE_PACK_MENU: UpdateChoosePackMenu(); break;
        case INIT_CHOOSE_LEVEL_MENU: UpdateInitChooseLevelMenu(); break;
        case CHOOSE_LEVEL_MENU: UpdateChooseLevelMenu(); break;
        case INIT_SETTINGS_MENU: UpdateInitSettingsMenu(); break;
        case SETTINGS_MENU: UpdateSettingsMenu(); break;
        case INIT_SCORES: UpdateInitScores(); break;
        case SCORES: UpdateScores(); break;
        case INIT_INITIALS: UpdateInitInitials(); break;
        case INITIALS: UpdateInitials(); break;
        case INIT_GAME: UpdateInitGame(); break;
        case GAME: UpdateGame(); break;
        default: break;
    }
}


void Engine::UpdateInitMainMenu()
{
    if (mSplash!=NULL)
    {
        SDL_FreeSurface(mSplash);
        mSplash=NULL;
    }

    if (!Mix_PlayingMusic()) Mix_PlayMusic(mThemeManager->mIntro, -1);

    mInputHandler.ResetKeys();

    mMenuMain = new MenuMain(&mInputHandler, mSurface, mThemeManager, mTextManager, mSettings.mControlMode, mSettings.mGameMode);
    mState = MAIN_MENU;
}


void Engine::UpdateMainMenu()
{
    if (mMenuMain->Update())
    {
        switch(mMenuMain->GetOption())
        {
            case 0: // Start
            {
                mState = INIT_CHOOSE_PACK_MENU;
                delete mMenuMain;
                mMenuMain = NULL;
                break;
            }
            case 1: // Hi-Scores
            {
                mState = INIT_SCORES;
                delete mMenuMain;
                mMenuMain = NULL;
                break;
            }
            case 2: // Settings
            {
                mState = INIT_SETTINGS_MENU;
                delete mMenuMain;
                mMenuMain = NULL;
                break;
            }
            case 3: // Quit
            {
                mQuit = true;
                break;
            }
        }
    }
}


void Engine::UpdateInitChoosePackMenu()
{
    mInputHandler.ResetKeys();

    mMenuChoosePack = new MenuChoosePack(&mInputHandler, mSurface, mThemeManager, mTextManager, mCurrentPack);
    mState = CHOOSE_PACK_MENU;
}


void Engine::UpdateChoosePackMenu()
{
    if (mMenuChoosePack->Update())
    {
        if (mMenuChoosePack->GetOption()==mMenuChoosePack->GetNumOptions()-1)
            mState = INIT_MAIN_MENU;
        else
        {
            strcpy(mCurrentPackFolder, mMenuChoosePack->GetCurrentPackFolder());
            mCurrentPack = mMenuChoosePack->GetCurrentPack();
            mLastPlayedSong = mMenuChoosePack->GetLastPlayedSong();
            if (mLastPlayedSong>0 && mSettings.mGameMode==0)
                mState = INIT_CHOOSE_LEVEL_MENU;
            else
                mState = INIT_GAME;
        }

        delete mMenuChoosePack;
        mMenuChoosePack = NULL;
    }
}


void Engine::UpdateInitChooseLevelMenu()
{
    mInputHandler.ResetKeys();

    mMenuChooseLevel = new MenuChooseLevel(&mInputHandler, mSurface, mThemeManager, mTextManager, mLastPlayedSong);
    mState = CHOOSE_LEVEL_MENU;
}


void Engine::UpdateChooseLevelMenu()
{
    if (mMenuChooseLevel->Update())
    {
        if (mMenuChooseLevel->GetOption()==mMenuChooseLevel->GetNumOptions()-1)
        {
            mInitialLevel=0;
            mState = INIT_CHOOSE_PACK_MENU;
        }
        else
        {
            mInitialLevel=mMenuChooseLevel->GetOption();
            mState=INIT_GAME;
        }

        delete mMenuChooseLevel;
        mMenuChooseLevel = NULL;
    }
}


void Engine::UpdateInitSettingsMenu()
{
    mInputHandler.ResetKeys();

    mMenuSettings = new MenuSettings(&mInputHandler, mSurface, mThemeManager, mTextManager, &mSettings);
    mState = SETTINGS_MENU;
}


void Engine::UpdateSettingsMenu()
{
    if (mMenuSettings->Update())
    {
        delete mMenuSettings;
        mMenuSettings = NULL;
        mState = INIT_MAIN_MENU;
    }
}


void Engine::UpdateInitScores()
{
    if (!Mix_PlayingMusic()) Mix_PlayMusic(mThemeManager->mIntro, -1);
    mInputHandler.ResetKeys();

    mMenuHiscores = new MenuHiscores(&mInputHandler, mSurface, mThemeManager, mTextManager, &mScoresManager, mCurrentPack);
    mState = SCORES;
}


void Engine::UpdateScores()
{
    if (mMenuHiscores->Update())
    {
        mCurrentPack=mMenuHiscores->GetCurrentPack();
        delete mMenuHiscores;
        mMenuHiscores=NULL;
        mState = INIT_MAIN_MENU;
    }
}


void Engine::UpdateInitInitials()
{
    mInputHandler.ResetKeys();
    mMenuEnterInitials = new MenuEnterInitials(&mInputHandler, mSurface, mThemeManager, mTextManager);
    mState = INITIALS;
}


void Engine::UpdateInitials()
{
    if (mMenuEnterInitials->Update())
    {
        mScoresManager.AddScore(mMenuEnterInitials->GetInitials(), mGame->GetScore(), mGame->GetLevel(), mCurrentPack);

        delete mMenuEnterInitials;
        mMenuEnterInitials=NULL;
        delete mGame;
        mGame = NULL;
        mState = INIT_SCORES;
    }
}


void Engine::UpdateInitGame()
{
    Mix_FadeOutMusic(500);
    mGame = new Game(mCurrentPackFolder, mSurface, mThemeManager, mTextManager, &mInputHandler, mSettings.mSpeed, mSettings.mControlMode, mSettings.mGameMode, mInitialLevel);
    mState=GAME;
}


void Engine::UpdateGame()
{
    bool quitGame=false;

    mGame->Update();

    if (!(mInputHandler.IsStart() && mInputHandler.IsSelect())) mTimeQuitGame = mMilliseconds;
    quitGame = mMilliseconds-mTimeQuitGame>1000;

    if (mGame->GetState()==Game::DEAD || quitGame)
    {
        Mix_HaltMusic();

        // Write last played song (only if greater that the current one)

        Uint8 lastPlayed=mGame->GetLevel()-1;
        Uint8 oldLastPlayed;

        char filename[256];
        sprintf(filename, "packs/%s/settings.dat", mCurrentPackFolder);
        FILE* packSettingsFile=fopen(filename, "rb");
        fread(&oldLastPlayed, sizeof(Uint8), 1, packSettingsFile);
        fclose(packSettingsFile);

        if (oldLastPlayed<lastPlayed && mSettings.mGameMode==0)
        {
            packSettingsFile=fopen(filename, "wb");
            fwrite(&lastPlayed, sizeof(Uint8), 1, packSettingsFile);
            fclose(packSettingsFile);
#ifdef TARGET_GP2X
            sync();
#endif
        }

        // Check if it's a highscore

        if (mScoresManager.IsAHiScore(mGame->GetScore(), mCurrentPack) && mSettings.mGameMode==0)
            mState = INIT_INITIALS;
        else
        {
            delete mGame;
            mGame = NULL;
            mState = INIT_MAIN_MENU;
        }
    }
}


void Engine::Draw()
{
//    static char fps[5];

    switch(mState)
    {
        case MAIN_MENU: mMenuMain->Draw(); break;
        case CHOOSE_PACK_MENU: mMenuChoosePack->Draw(); break;
        case CHOOSE_LEVEL_MENU: mMenuChooseLevel->Draw(); break;
        case SETTINGS_MENU: mMenuSettings->Draw(); break;
        case SCORES: mMenuHiscores->Draw(); break;
        case INITIALS: mMenuEnterInitials->Draw(); break;
        case GAME: mGame->Draw(); break;
        default: break;
    }

    if (mShowVolume)
    {
        if (SDL_GetTicks()-mTimeVolume<2000)
        {
            SDL_Rect volumeRect;
            volumeRect.h=4;
            volumeRect.y=RESY-6;

            volumeRect.x=107;
            volumeRect.w=2;
            SDL_FillRect(mSurface, &volumeRect, SDL_MapRGB(mSurface->format, 0, 255, 0));

            volumeRect.x=211;
            volumeRect.w=2;
            SDL_FillRect(mSurface, &volumeRect, SDL_MapRGB(mSurface->format, 0, 255, 0));

            volumeRect.x=110;
            volumeRect.y=RESY-6;
            volumeRect.w=mVolume==0?1:(100*mVolume)/MIX_MAX_VOLUME;
            volumeRect.h=4;
            SDL_FillRect(mSurface, &volumeRect, SDL_MapRGB(mSurface->format, 0, 255, 0));
        }
        else mShowVolume=false;
    }

//    sprintf(fps, "%d", mFPSCounter.GetFPS());
//    mTextManager->Write(RESX, RESY>>1, fps, RIGHT|DOWN);

    SDL_Flip(mSurface);
}
