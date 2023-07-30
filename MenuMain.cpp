#include "MenuMain.h"
#include "Consts.h"


MenuMain::MenuMain(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 controlMode, Uint8 gameMode) : Menu(inputHandler, surface, themeManager, textManager, 0, true)
{
    mOriginY=128;

    mOptionNames.push_back(new char[6]);
    strcpy(mOptionNames[0], "START");
    mOptionNames.push_back(new char[10]);
    strcpy(mOptionNames[1], "HI-SCORES");
    mOptionNames.push_back(new char[256]);
    strcpy(mOptionNames[2], "SETTINGS");
    mOptionNames.push_back(new char[5]);
    strcpy(mOptionNames[3], "QUIT");

    mMaxOptionsPerPage=6;
}


MenuMain::~MenuMain()
{
}
