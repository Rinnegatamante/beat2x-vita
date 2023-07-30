#include "MenuEnterInitials.h"
#include "Consts.h"
#include "SDL_image.h"
#include <iostream>

using namespace std;


MenuEnterInitials::MenuEnterInitials(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager) : Menu(inputHandler, surface, themeManager, textManager, 0, false)
{
    mTitle = new char[15];
    strcpy(mTitle, "ENTER INITIALS");

    mKeyInitDelay=12;
    mKeyDelay = mKeyInitDelay;

    mInitialPos=0;
    mCursorPosX=0;
    mCursorPosY=1;

    mInitials[0]=mInitials[1]=mInitials[2]=mInitials[3]=' ';
    strcpy(mCharArray[0], "0123456789");
    strcpy(mCharArray[1], "ABCDEFGHIJ");
    strcpy(mCharArray[2], "KLMNOPQRST");
    strcpy(mCharArray[3], "UVWXYZ   [");

    mTimerCursor=SDL_GetTicks();
}


MenuEnterInitials::~MenuEnterInitials()
{
}


bool MenuEnterInitials::Update()
{
    if (mKeyDelay>0) mKeyDelay--;
    if (mKeyDelay==0)
    {
        if (mInputHandler->IsUp())
        {
            if (mCursorPosY>0)
                mCursorPosY--;
            else
                mCursorPosY=3;

            mKeyDelay=mKeyInitDelay;
        }
        if (mInputHandler->IsDown())
        {
            if (mCursorPosY<3)
                mCursorPosY++;
            else
                mCursorPosY=0;

            mKeyDelay=mKeyInitDelay;
        }
        if (mInputHandler->IsLeft())
        {
            if (mCursorPosX>0)
                mCursorPosX--;
            else
                mCursorPosX=9;

            mKeyDelay=mKeyInitDelay;
        }
        else if (mInputHandler->IsRight())
        {
            if (mCursorPosX<9)
                mCursorPosX++;
            else
                mCursorPosX=0;

            mKeyDelay=mKeyInitDelay;
        }

        if (mInputHandler->IsA() || mInputHandler->IsB() || mInputHandler->IsX() || mInputHandler->IsY())
        {
            if (mInitialPos==3 && mInitials[mInitialPos]!='[')
            {
                mInitials[3]='\0';
                return true;
            }
            else
            {
                if (mInitials[mInitialPos]!='[')
                    mInitialPos++;
                else
                {
                    if (mInitialPos>0)
                    {
                        mInitials[mInitialPos]=' ';
                        mInitialPos--;
                    }
                }

            }

            mKeyDelay=mKeyInitDelay;
        }
    }

    mInitials[mInitialPos]=mCharArray[mCursorPosY][mCursorPosX];

    return false;
}


void MenuEnterInitials::Draw()
{
    static char sInitials[6], sChar[2];
    SDL_BlitSurface(mThemeManager->mMenuBackground, NULL, mSurface, NULL);

    mTextManager->Write(RESX>>1, 70, mTitle, HCENTRE|VCENTRE);
    if (mInitials[3]!='[')
        sprintf(sInitials, "%c %c %c  ", mInitials[0], mInitials[1], mInitials[2]);
    else
        sprintf(sInitials, "%c %c %c %c", mInitials[0], mInitials[1], mInitials[2], mInitials[3]);
    mTextManager->Write((RESX>>1)+CHAR_SIZE, 104, sInitials, HCENTRE|VCENTRE);

    if (SDL_GetTicks()-mTimerCursor<500)
    {
        SDL_Rect initialPos, initialRect;
        initialPos.x=137+(mInitialPos*CHAR_SIZE<<1); initialPos.y=97;
        initialRect.x=13; initialRect.y=0; initialRect.w=initialRect.h=13;
        SDL_BlitSurface(mThemeManager->mCharSelector, &initialRect, mSurface, &initialPos);
    }
    else
    {
        if (SDL_GetTicks()-mTimerCursor>=1000)
            mTimerCursor=SDL_GetTicks();
    }

    Uint16 xInit=84;
    Uint16 yInit=140;
    for (Uint8 i=0; i<10; i++)
        for (Uint8 j=0; j<4; j++)
        {
            sprintf(sChar, "%c", mCharArray[j][i]);
            mTextManager->Write(xInit+(i*CHAR_SIZE<<1), yInit+(j*CHAR_SIZE<<1), sChar, HCENTRE|VCENTRE);
        }

    SDL_Rect charPos, charRect;
    charPos.x=xInit-(CHAR_SIZE>>1)-3+mCursorPosX*(CHAR_SIZE<<1); charPos.y=yInit-(CHAR_SIZE>>1)-3+mCursorPosY*(CHAR_SIZE<<1);
    charRect.x=charRect.y=0; charRect.w=charRect.h=13;
    SDL_BlitSurface(mThemeManager->mCharSelector, &charRect, mSurface, &charPos);
}
