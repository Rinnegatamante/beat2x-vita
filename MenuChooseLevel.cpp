#include "MenuChooseLevel.h"


MenuChooseLevel::MenuChooseLevel(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 lastLevel) : Menu(inputHandler, surface, themeManager, textManager)
{
    mOriginY=100;

    mTitle = new char[17];
    strcpy(mTitle, "CHOOSE LEVEL");

    for (Uint8 i=0; i<=lastLevel; i++)
    {
        mOptionNames.push_back(new char[9]);
        sprintf(mOptionNames[i], "LEVEL %2d", i+1);
    }

    mOptionNames.push_back(new char[7]);
    strcpy(mOptionNames[lastLevel+1], "(BACK)");
}


MenuChooseLevel::~MenuChooseLevel()
{
}

