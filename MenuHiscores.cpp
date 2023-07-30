#include "MenuHiscores.h"
#include "Consts.h"

#include <iostream>

using namespace std;



MenuHiscores::MenuHiscores(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, ScoresManager* scoresManager, Uint8 currentPack) : Menu(inputHandler, surface, themeManager, textManager)
{
    mTitle = new char[17];
    strcpy(mTitle, "HI-SCORES");

    mScoresManager=scoresManager;
    mScoresManager->ReadFile();
    mCurrentPack=currentPack;
}


MenuHiscores::~MenuHiscores()
{
}


bool MenuHiscores::Update()
{
    if (mKeyDelay>0) mKeyDelay--;
    if (mKeyDelay==0)
    {
        if (mInputHandler->IsLeft())
        {
            if (mCurrentPack==0)
                mCurrentPack=mScoresManager->GetNumPacks()-1;
            else
                mCurrentPack--;

            mKeyDelay=mKeyInitDelay;
        }
        else if (mInputHandler->IsRight())
        {
            mCurrentPack++;
            if (mCurrentPack==mScoresManager->GetNumPacks())
                mCurrentPack=0;

            mKeyDelay=mKeyInitDelay;
        }
        if (mInputHandler->IsA() || mInputHandler->IsB() || mInputHandler->IsX() || mInputHandler->IsY())
            return true;
    }

    return false;
}


void MenuHiscores::Draw()
{
    static char sCurrentPack[256];
    static char sRow[100];

    SDL_BlitSurface(mThemeManager->mMenuBackground, NULL, mSurface, NULL);

    mTextManager->Write(RESX>>1, 54, mTitle, HCENTRE|VCENTRE);
    sprintf(sCurrentPack, "%s (%d/%d)", mScoresManager->GetPackName(mCurrentPack), mCurrentPack+1, mScoresManager->GetNumPacks());
    Uint16 length = strlen(sCurrentPack);
    for (Uint16 i=0; i<length; i++)
        sCurrentPack[i]=toupper(sCurrentPack[i]);
    mTextManager->Write(RESX>>1, 64, sCurrentPack, HCENTRE|VCENTRE);

    ScoresManager::Row* row;

    mTextManager->Write(76, 85, "POS NAME SCORE   LEVEL", VCENTRE);
    for (Uint8 i=0; i<10; i++)
    {
        row = mScoresManager->GetRow(mCurrentPack, i);

        sprintf(sRow, " %2d %3s  %07d %02d", i+1, row->initials, row->score, row->level);
        mTextManager->Write(76, 99+i*(CHAR_SIZE+2), sRow, VCENTRE);
    }

    if (mScoresManager->GetNumPacks()>1)
        mTextManager->Write(RESX>>1, 210, "[,\\: CHANGE PACK     A,B,X,Y: BACK", HCENTRE|VCENTRE);
    else
        mTextManager->Write(RESX>>1, 210, "A,B,X,Y: BACK", HCENTRE|VCENTRE);
}
