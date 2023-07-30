#include "Menu.h"
#include "Consts.h"

#include <iostream>
using namespace std;


Menu::Menu(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 defaultOption, bool altBackground)
{
    mInputHandler = inputHandler;
    mTextManager = textManager;
    mThemeManager = themeManager;
    mTitle = NULL;
    mOriginY = 0;
    mCurrentOption = defaultOption;
    mMaxOptionsPerPage = 6;
    mKeyInitDelay = 15;
    mKeyDelay = mKeyInitDelay;
    mSurface = surface;

    mAltBackground = altBackground;
}


Menu::~Menu()
{
    if (mTitle!=NULL) delete mTitle;
    for (Uint8 i=0; i<mOptionNames.size(); i++) delete mOptionNames[i];
    mOptionNames.clear();
}


void Menu::NextOption()
{
    mCurrentOption++;
    if (mCurrentOption>=mOptionNames.size()) mCurrentOption=0;
}


void Menu::PreviousOption()
{
    if (mCurrentOption==0)
        mCurrentOption=mOptionNames.size()-1;
    else
        mCurrentOption--;
}


void Menu::Draw()
{
    static char optionName[41];

    if (!mAltBackground)
        SDL_BlitSurface(mThemeManager->mMenuBackground, NULL, mSurface, NULL);
    else
        SDL_BlitSurface(mThemeManager->mMainMenuBackground, NULL, mSurface, NULL);

    mTextManager->Write(RESX>>1, mOriginY - (CHAR_SIZE<<2), mTitle, HCENTRE|VCENTRE);

    Sint8 start=0;
    Sint8 end=mOptionNames.size();

    if (mCurrentOption>mMaxOptionsPerPage-1) start=mCurrentOption-mMaxOptionsPerPage+1;

    for (Sint8 i=start; i<end; i++)
    {
        if (i-start<mMaxOptionsPerPage)
        {
            sprintf(optionName, "%s", mOptionNames[i]);
            for (Uint16 j=0; j<strlen(optionName); j++)
                optionName[j]=toupper(optionName[j]);

            if (i==mCurrentOption)
                mTextManager->Write(RESX>>1, mOriginY + ((i-start)*CHAR_SIZE<<1), optionName, HCENTRE|VCENTRE, true);
            else
                mTextManager->Write(RESX>>1, mOriginY + ((i-start)*CHAR_SIZE<<1), optionName, HCENTRE|VCENTRE, false);

        }
    }

    if (start>0) mTextManager->Write(RESX-(CHAR_SIZE<<1), mOriginY, "]", HCENTRE|VCENTRE);
    if (end-start>mMaxOptionsPerPage) mTextManager->Write(RESX-(CHAR_SIZE<<1), mOriginY+(mMaxOptionsPerPage-1)*(CHAR_SIZE<<1), "^", HCENTRE|VCENTRE);
}


bool Menu::Update()
{
    if (mKeyDelay>0) mKeyDelay--;
    if (mKeyDelay==0)
    {
        if (mInputHandler->IsUp())
        {
            PreviousOption();
            mKeyDelay=mKeyInitDelay;
        }
        else if (mInputHandler->IsDown())
        {
            NextOption();
            mKeyDelay=mKeyInitDelay;
        }
        if (mInputHandler->IsA() || mInputHandler->IsB() || mInputHandler->IsX() || mInputHandler->IsY())
            return true;
    }

    return false;
}
