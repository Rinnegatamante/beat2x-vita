#include "TextManager.h"

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>


using namespace std;


TextManager::TextManager(ThemeManager* themeManager, SDL_Surface* surface)
{
    mSurface = surface;
    mThemeManager = themeManager;
}


TextManager::~TextManager()
{
    list<TextElement>::iterator it;
    for (it=mTextList.begin(); it!=mTextList.end(); it++)
        free((*it).mText);
    mTextList.clear();
}


void TextManager::Write(Sint16 x, Sint16 y, char* text, int flags, bool alt)
{
    if (text!=NULL)
    {
        mLength = strlen(text);

        if (mLength>0)
        {
            mX = x; mY = y;
            if (flags&0x01000000) mX -= mLength*CHAR_SIZE;
            if (flags&0x00100000) mX -= mLength*CHAR_SIZE/2;
            if (flags&0x00001000) mY -= CHAR_SIZE;
            if (flags&0x00000100) mY -= CHAR_SIZE/2;

            mChar.y = mY;
            mCharInImage.y = alt?CHAR_SIZE:0;
            mCharInImage.w = mCharInImage.h = CHAR_SIZE;

            for (Uint8 i=0; i<mLength; i++)
            {
                mChar.x = mX + (i*CHAR_SIZE);
                mChar.w = mLength*CHAR_SIZE*2;
                mChar.h = CHAR_SIZE*2;
                mCharInImage.x = (text[i]-32) * CHAR_SIZE;
                SDL_BlitSurface(mThemeManager->mFont, &mCharInImage, mSurface, &mChar);
            }
        }
    }
}


void TextManager::AddTextElement(Sint16 xfrom, Sint16 yfrom, Sint16 xto, Sint16 yto, char* text, Uint32 time, int flags)
{
    TextElement textElement;
    textElement.mXFrom=xfrom;
    textElement.mYFrom=yfrom;
    textElement.mXTo=xto;
    textElement.mYTo=yto;
    textElement.mText=(char*)(malloc(sizeof(char)*(strlen(text)+1)));
    strcpy(textElement.mText, text);
    textElement.mTotalTime=time;
    textElement.mFlags=flags;
    textElement.mStartTime=SDL_GetTicks();
    mTextList.push_back(textElement);
}


void TextManager::Update()
{
    list<TextElement>::iterator it;
    Uint32 time=SDL_GetTicks();

    for (it=mTextList.begin(); it!=mTextList.end();)
    {
        if (time-(*it).mStartTime<=(*it).mTotalTime)
        {
            (*it).mX = (*it).mXFrom + ((*it).mXTo-(*it).mXFrom)*((Sint32)time-(Sint32)((*it).mStartTime))/(Sint32)((*it).mTotalTime);
            (*it).mY = (*it).mYFrom + ((*it).mYTo-(*it).mYFrom)*((Sint32)time-(Sint32)((*it).mStartTime))/(Sint32)((*it).mTotalTime);
            it++;
        }
        else
        {
            free((*it).mText);
            mTextList.erase(it++);
        }
    }
}


void TextManager::Draw()
{
    list<TextElement>::iterator it;
    for (it=mTextList.begin(); it!=mTextList.end(); it++)
    {
        SDL_SetAlpha(mThemeManager->mFont, SDL_SRCALPHA, 255-(((SDL_GetTicks()-(*it).mStartTime)*255)/(*it).mTotalTime));
        Write((*it).mX, (*it).mY, (*it).mText, (*it).mFlags);
        SDL_SetAlpha(mThemeManager->mFont, SDL_SRCALPHA, 255);
    }
}
