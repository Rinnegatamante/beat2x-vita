#include "Game.h"
#include <iostream>

using namespace std;


Game::Game(char* pack, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, InputHandler* inputHandler, Uint8 speed, Uint8 controlMode, Uint8 gameMode, Uint8 initialLevel)
{
    mSurface = surface;
    mThemeManager = themeManager;
    mTextManager = textManager;
    mInputHandler = inputHandler;
    mGameMode = gameMode;
    mLevel = (mGameMode==0)?initialLevel:0;
    mSpeed = speed;
    mMaxCombo=0;

    mControlMode=controlMode;

    mPack=(char*)(malloc(sizeof(char)*(strlen(pack)+1)));
    strcpy(mPack, pack);

    mScore = mScoreTmp = 0;

    // Load song list file

    mNumSongs = 0;

    char songListFile[256];
    sprintf(songListFile, "packs/%s/songlist.bxs", mPack);

    FILE* hSongListFile = fopen(songListFile, "rt");
    if (hSongListFile == NULL)
    {
        cerr << "Error reading <" << songListFile << ">: " << SDL_GetError() << endl;
        exit(1);
    }

    char line[256];
    while ((fgets(line, 256, hSongListFile)!=NULL) && (mNumSongs<MAX_SONGS))
    {
        if (strlen(line)>1 && line[0]!='#')
        {
            mSongList[mNumSongs] = (char*)(malloc(sizeof(char)*(strlen(line))));
            for (Uint16 i=0; i<strlen(line); i++)
                mSongList[mNumSongs][i]=line[i];
            mSongList[mNumSongs][strlen(line)-1]='\0';
            mNumSongs++;
        }
    }

    fclose(hSongListFile);

    mREnergyBarPos.x=0; mREnergyBarPos.y=0;
    mREnergyBarRect.x=0; mREnergyBarRect.y=0;
    mREnergyBarRect.w=320; mREnergyBarRect.h=8;
    mEnergyBarPos.x=0; mEnergyBarPos.y=0;
    mEnergyBarRect.x=0; mEnergyBarRect.y=8;
    mEnergyBarRect.h=8;

    mScorePos.x=10; mScorePos.y=RESY-22;
    mScoreRect.x=0; mScoreRect.y=0; mScoreRect.w=33; mScoreRect.h=10;
    mLevelPos.x=RESX-40; mLevelPos.y=RESY-22;
    mLevelRect.x=0; mLevelRect.y=10; mLevelRect.w=30; mLevelRect.h=10;

    mMessagesRect.x=0;
    mMessagesRect.w=150;
    mMessagesRect.h=35;
    mMessagesPos.x=(RESX>>1)-75;
    mMessagesPos.y=(RESY>>1)-8;

    mComboNumbersRect.y=0;
    mComboNumbersRect.w=22;
    mComboNumbersRect.h=28;

    mMessageFirstFrame=true;
    mComboDisplX=0;
    mComboDisplY=0;

    mCurrentSong = NULL;
    mSongStatistics = NULL;
    mState = INIT_LEVEL;

    mTimeShowMessage = 0;
}


Game::~Game()
{
    if (mCurrentSong!=NULL)
    {
        delete mCurrentSong;
        mCurrentSong=NULL;
    }
    for (Uint8 i=0; i<mNumSongs; i++) free(mSongList[i]);
    free(mPack);
}


void Game::Update()
{
    switch(mState)
    {
        case INIT_LEVEL: UpdateInitLevel(); break;
        case COUNTDOWN_1: UpdateCountdown1(); break;
        case COUNTDOWN_2: UpdateCountdown2(); break;
        case COUNTDOWN_3: UpdateCountdown3(); break;
        case LEVEL: UpdateLevel(); break;
        case STATISTICS: UpdateStatistics(); break;
        case END_LEVEL: UpdateEndLevel(); break;
        case RESULTS: UpdateResults(); break;
        case INIT_END_GAME: UpdateInitEndGame(); break;
        case END_GAME: UpdateEndGame(); break;
        default: break;
    }
}


void Game::UpdateInitLevel()
{
    static char level[8];

    mStopCounting=false;

    mEnergy = 500000;
    mEnergyTmp = 0;
    mCombo=0;
    mLevelMaxCombo=0;
    mScoreTmp=mScore;

    if (mLevel<mNumSongs)
    {
        char songName[128];
        sprintf(songName, "packs/%s/%s", mPack, mSongList[mLevel]);
        switch(mSpeed)
        {
            case 0: mCurrentSong = new Song(songName, mThemeManager->mButtonSet, mSurface, 3000); break;
            case 1: mCurrentSong = new Song(songName, mThemeManager->mButtonSet, mSurface, 1500); break;
            case 2: mCurrentSong = new Song(songName, mThemeManager->mButtonSet, mSurface, 800); break;
            default: break;
        }
        mSongStatistics = new SongStatistics(mCurrentSong->GetNumEvents(), mSurface, mThemeManager->mStatisticsBackground, mThemeManager->mStatisticsBars, mTextManager, mInputHandler, this);

        mKeysEnabled = true;

        sprintf(level, "LEVEL %d", mLevel+1);
        mTextManager->AddTextElement(RESX>>1, (RESY>>1)+15-(CHAR_SIZE<<2), RESX>>1, (RESY>>1)+15-(CHAR_SIZE<<2)-40, level, 4600, HCENTRE|VCENTRE);

        mTextManager->AddTextElement(RESX>>1, (RESY>>1)+15, RESX>>2, (RESY>>1)+15, "READY", 3000, HCENTRE|VCENTRE);
        Mix_PlayChannel(-1, mThemeManager->mSoundCountdown, 0);
        mState = COUNTDOWN_1;
        mCountdown = SDL_GetTicks();
    }
    else
    {
        mLevel--;
        mState = INIT_END_GAME;
    }
}


void Game::UpdateCountdown1()
{
    mTextManager->Update();
    if (SDL_GetTicks()-mCountdown>1500)
    {
        Mix_PlayChannel(-1, mThemeManager->mSoundCountdown, 0);
        mTextManager->AddTextElement(RESX>>1, (RESY>>1)+15 + CHAR_SIZE, (RESX*3)>>2, (RESY>>1)+15 + CHAR_SIZE, "SET", 3000, HCENTRE|VCENTRE);
        mState = COUNTDOWN_2;
    }
}


void Game::UpdateCountdown2()
{
    mTextManager->Update();
    if (SDL_GetTicks()-mCountdown>3000)
    {
        Mix_PlayChannel(-1, mThemeManager->mSoundGo, 0);
        mTextManager->AddTextElement(RESX>>1, (RESY>>1)+15+(CHAR_SIZE<<1), RESX>>1, (RESY>>1)+15+(CHAR_SIZE<<1), "MOVE YOUR FINGERS!", 1550, HCENTRE|VCENTRE);
        mState = COUNTDOWN_3;
    }
}


void Game::UpdateCountdown3()
{
    mTextManager->Update();
    mState = LEVEL;
    mCurrentSong->Start();
}


void Game::UpdateLevel()
{
    Uint8 keysPressed=0;

    switch(mControlMode)
    {
        case 0:
        {
            if (mInputHandler->IsA()) keysPressed=keysPressed|(1);
            if (mInputHandler->IsB()) keysPressed=keysPressed|(1<<1);
            if (mInputHandler->IsX()) keysPressed=keysPressed|(1<<2);
            if (mInputHandler->IsY()) keysPressed=keysPressed|(1<<3);
            break;
        }
        case 1:
        {
            if (mInputHandler->IsLeft()||mInputHandler->IsUpLeft()) keysPressed=keysPressed|(1);
            if (mInputHandler->IsRight()||mInputHandler->IsDownRight()) keysPressed=keysPressed|(1<<1);
            if (mInputHandler->IsDown()||mInputHandler->IsDownLeft()) keysPressed=keysPressed|(1<<2);
            if (mInputHandler->IsUp()||mInputHandler->IsUpRight()) keysPressed=keysPressed|(1<<3);
            break;
        }
        case 2:
        {
            if (mInputHandler->IsA()||mInputHandler->IsLeft()||mInputHandler->IsUpLeft()) keysPressed=keysPressed|(1);
            if (mInputHandler->IsB()||mInputHandler->IsRight()|mInputHandler->IsDownRight()) keysPressed=keysPressed|(1<<1);
            if (mInputHandler->IsX()||mInputHandler->IsDown()||mInputHandler->IsDownLeft()) keysPressed=keysPressed|(1<<2);
            if (mInputHandler->IsY()||mInputHandler->IsUp()||mInputHandler->IsUpRight()) keysPressed=keysPressed|(1<<3);

            break;
        }
    }

    if (mInputHandler->IsL()) keysPressed=keysPressed|(1<<4);
    if (mInputHandler->IsR()) keysPressed=keysPressed|(1<<5);

    if (keysPressed==0 && !mKeysEnabled)
    {
        // Empties the event queue
        SDL_Event event;
        while(SDL_PollEvent(&event));

        mKeysEnabled=true;
    }
    if (keysPressed!=0 && !mKeysEnabled) keysPressed=0;

    Sint8 result=mCurrentSong->Update(keysPressed);
    if (keysPressed!=0) mKeysEnabled=false;

    switch(result)
    {
        case 1: // Perfect
        {
            if (!mStopCounting)
            {
                mScore+=18;
                mEnergy+=1000000/mCurrentSong->GetNumEvents();
                if (mEnergy>1000000) mEnergy=1000000;
            }

            mCombo++;
            mLevelMaxCombo = (mCombo>mLevelMaxCombo && mCombo>3)?mCombo:mLevelMaxCombo;
            mMaxCombo = (mLevelMaxCombo>mMaxCombo)?mLevelMaxCombo:mMaxCombo;
            mMessageFirstFrame=true;

            mSongStatistics->AddPerfect();
            mSongStatistics->SetCombo(mLevelMaxCombo);
            mMessagesRect.y=0;
            mTimeShowMessage = SDL_GetTicks();
            mMessagesPos.y=(RESY>>1)-8;
            break;
        }
        case 2: // Great
        {
            if (!mStopCounting)
            {
                mScore+=15;
                mEnergy+=800000/mCurrentSong->GetNumEvents();
                if (mEnergy>1000000) mEnergy=1000000;
            }

            mCombo++;
            mLevelMaxCombo = (mCombo>mLevelMaxCombo && mCombo>3)?mCombo:mLevelMaxCombo;
            mMaxCombo = (mLevelMaxCombo>mMaxCombo)?mLevelMaxCombo:mMaxCombo;
            mMessageFirstFrame=true;

            mSongStatistics->AddGreat();
            mSongStatistics->SetCombo(mMaxCombo);
            mMessagesRect.y=35;
            mTimeShowMessage = SDL_GetTicks();
            mMessagesPos.y=(RESY>>1)-8;
            break;
        }
        case 3: // Good
        {
            if (!mStopCounting)
            {
                mScore+=10;
                mEnergy+=500000/mCurrentSong->GetNumEvents();
                if (mEnergy>1000000) mEnergy=1000000;
            }

            mCombo=0;

            mSongStatistics->AddGood();
            mMessagesRect.y=70;
            mTimeShowMessage = SDL_GetTicks();
            mMessagesPos.y=(RESY>>1)-8;
            break;
        }
        case 4: // Poor
        case 5: // Missed
        {
            if (!mStopCounting)
            {
                mEnergy-=60000;
                if (mEnergy<0) mEnergy=0;
            }
            if (result==4)
                mSongStatistics->AddPoor();
            else
                mSongStatistics->AddMissed();

            mCombo=0;

            mMessagesRect.y=105;
            mTimeShowMessage = SDL_GetTicks();
            mMessagesPos.y=(RESY>>1)-8;
            break;
        }
        default: break;
    }

    if (mScoreTmp<mScore)
    {
        mScoreTmp+=2;
        if (mScoreTmp>mScore) mScoreTmp=mScore;
    }
    else if (mScoreTmp>mScore)
    {
        mScoreTmp-=2;
        if (mScoreTmp<mScore) mScoreTmp=mScore;
    }

    if (mEnergyTmp<mEnergy)
    {
        mEnergyTmp+=10000;
        if (mEnergyTmp>mEnergy) mEnergyTmp=mEnergy;
    }
    else if (mEnergyTmp>mEnergy)
    {
        mEnergyTmp-=10000;
        if (mEnergyTmp<mEnergy) mEnergyTmp=mEnergy;
    }

    mTextManager->Update();

    if (mCurrentSong->HasEnded())
    {
        Mix_HaltMusic();
        Mix_FadeInMusic(mThemeManager->mMusicStatistics, 1, 1000);
        mState=STATISTICS;
    }
    if (!mStopCounting && mEnergy==0)
    {
        mStopCounting=true;
        mTimerFailed=SDL_GetTicks();
    }
}


void Game::UpdateStatistics()
{
    if (mSongStatistics->GetState()!=SongStatistics::DONE)
        mSongStatistics->Update();
    else
    {
        Mix_FadeOutMusic(2000);
        mState = END_LEVEL;
    }
}


void Game::UpdateEndLevel()
{
    delete mCurrentSong;
    mCurrentSong=NULL;
    delete mSongStatistics;
    mSongStatistics=NULL;
    if (!mStopCounting) mLevel++;
    mTimeResults = SDL_GetTicks();
    mState = RESULTS;
}


void Game::UpdateResults()
{
    if (SDL_GetTicks()-mTimeResults>2000)
        mState = mStopCounting ? INIT_END_GAME : INIT_LEVEL;
}


void Game::UpdateInitEndGame()
{
    if (mCurrentSong!=NULL)
    {
        delete mCurrentSong;
        mCurrentSong=NULL;
    }
    if (mSongStatistics!=NULL)
    {
        delete mSongStatistics;
        mSongStatistics=NULL;
    }
    mState = END_GAME;
}


void Game::UpdateEndGame()
{
    mState=DEAD;
}


void Game::Draw()
{
    switch(mState)
    {
        case COUNTDOWN_1:
        case COUNTDOWN_2:
        case COUNTDOWN_3:
        case LEVEL: DrawLevel(); break;
        case STATISTICS: DrawStatistics(); break;
        case RESULTS:
        {
            if (!mStopCounting)
                SDL_BlitSurface(mThemeManager->mCleared, NULL, mSurface, NULL);
            else
                SDL_BlitSurface(mThemeManager->mFailed, NULL, mSurface, NULL);
            break;
        }
        default: break;
    }
}


void Game::DrawLevel()
{
    static char score[8], level[3];
    Uint32 time=SDL_GetTicks();

    SDL_BlitSurface(mThemeManager->mGameBackground, NULL, mSurface, NULL);

    SDL_BlitSurface(mThemeManager->mBoardText, &mScoreRect, mSurface, &mScorePos);
    sprintf(score, "%07d", mScoreTmp);
    mTextManager->Write(10, RESY-10, score);

    SDL_BlitSurface(mThemeManager->mBoardText, &mLevelRect, mSurface, &mLevelPos);
    sprintf(level, "%02d", mLevel+1);
    mTextManager->Write(RESX-25, RESY-10, level);

    SDL_BlitSurface(mThemeManager->mEnergyBar, &mREnergyBarRect, mSurface, &mREnergyBarPos);
    mEnergyBarRect.w=(320*mEnergyTmp)/1000000;
    SDL_BlitSurface(mThemeManager->mEnergyBar, &mEnergyBarRect, mSurface, &mEnergyBarPos);

    if (time-mTimeShowMessage<700)
    {
        SDL_SetAlpha(mThemeManager->mMessages, SDL_SRCALPHA, 256-(((time-mTimeShowMessage)*256)/700));
        SDL_BlitSurface(mThemeManager->mMessages, &mMessagesRect, mSurface, &mMessagesPos);
        mMessagesPos.y=(RESY>>1)-8-(time-mTimeShowMessage)*10/700;

        if (mCombo>3)
        {
            Uint16 hundreds=(mCombo/100);
            Uint16 tens=(mCombo-hundreds*100)/10;
            Uint16 units=mCombo-hundreds*100-tens*10;

            Uint16 xIniCombo = 96;
            if (hundreds==0) xIniCombo-=15;
            if (hundreds==0 && tens==0) xIniCombo-=15;

            if (mMessageFirstFrame)
            {
                mComboDisplX=(rand()%18)-9;
                if (mComboDisplX<0)
                    mComboDisplX-=5;
                else
                    mComboDisplX+=5;
                mComboDisplY=(rand()%18)-9;
                if (mComboDisplY<0)
                    mComboDisplY-=5;
                else
                    mComboDisplY+=5;

                mComboPos1.x=xIniCombo+mComboDisplX;
                mComboPos1.y=170+mComboDisplY;

                mMessageFirstFrame=false;
            }
            else
            {
                mComboPos1.x += (xIniCombo-mComboPos1.x)/5;
                mComboPos1.y += (170-mComboPos1.y)/5;
            }

            mComboPos2.y=mComboPos1.y;

            if (hundreds!=0)
            {
                mComboNumbersRect.x = hundreds*22;
                SDL_BlitSurface(mThemeManager->mComboNumbers, &mComboNumbersRect, mSurface, &mComboPos1);
            }
            mComboPos2.x=mComboPos1.x+20;
            if (hundreds>0 || (hundreds==0 && tens>0))
            {
                mComboNumbersRect.x = tens*22;
                SDL_BlitSurface(mThemeManager->mComboNumbers, &mComboNumbersRect, mSurface, &mComboPos2);
            }
            mComboNumbersRect.x = units*22;
            mComboPos2.x+=20;
            SDL_BlitSurface(mThemeManager->mComboNumbers, &mComboNumbersRect, mSurface, &mComboPos2);
            mComboPos2.x+=20;
            SDL_BlitSurface(mThemeManager->mCombo, NULL, mSurface, &mComboPos2);
        }
    }

    if (mStopCounting)
    {
        if (time-mTimerFailed<1000)
            mTextManager->Write(RESX>>1, RESY-1, "FAILED", HCENTRE|DOWN);
        else
            if (time-mTimerFailed>=2000)
                mTimerFailed=time;
    }

    mCurrentSong->Draw();
    mTextManager->Draw();
}


void Game::DrawStatistics()
{
    static char score[8], level[3];

    mSongStatistics->Draw();

    SDL_BlitSurface(mThemeManager->mBoardText, &mScoreRect, mSurface, &mScorePos);
    sprintf(score, "%07d", mScore);
    mTextManager->Write(10, RESY-10, score);

    SDL_BlitSurface(mThemeManager->mBoardText, &mLevelRect, mSurface, &mLevelPos);
    sprintf(level, "%02d", mLevel+1);
    mTextManager->Write(RESX-25, RESY-10, level);

    SDL_BlitSurface(mThemeManager->mEnergyBar, &mREnergyBarRect, mSurface, &mREnergyBarPos);
    mEnergyBarRect.w=(320*mEnergy)/1000000;
    SDL_BlitSurface(mThemeManager->mEnergyBar, &mEnergyBarRect, mSurface, &mEnergyBarPos);
}
