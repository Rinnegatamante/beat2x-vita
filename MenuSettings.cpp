#include "MenuSettings.h"


MenuSettings::MenuSettings(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Settings* settings) : Menu(inputHandler, surface, themeManager, textManager)
{
    mSettings = settings;
    mUpdated = false;

    mOriginY=120;

    mTitle = new char[9];
    strcpy(mTitle, "SETTINGS");

    mOptionNames.push_back(new char[20]);
    switch(mSettings->mGameMode)
    {
        case 0: strcpy(mOptionNames[0], "GAME MODE: NORMAL"); break;
        case 1: strcpy(mOptionNames[0], "GAME MODE: PRACTICE"); break;
    }

    mOptionNames.push_back(new char[23]);
    switch(mSettings->mControlMode)
    {
        case 0: strcpy(mOptionNames[1], "CONTROL MODE: BUTTONS"); break;
        case 1: strcpy(mOptionNames[1], "CONTROL MODE: STICK"); break;
        case 2: strcpy(mOptionNames[1], "CONTROL MODE: COMBINED"); break;
    }

    mOptionNames.push_back(new char[14]);
    switch(mSettings->mSpeed)
    {
        case 0: strcpy(mOptionNames[2], "SPEED: SLOW"); break;
        case 1: strcpy(mOptionNames[2], "SPEED: NORMAL"); break;
        case 2: strcpy(mOptionNames[2], "SPEED: FAST"); break;
    }

    mOptionNames.push_back(new char[256]);
    sprintf(mOptionNames[3], "THEME: %s", mThemeManager->GetCurrentThemeName());

    mOptionNames.push_back(new char[7]);
    strcpy(mOptionNames[4], "(BACK)");
}


MenuSettings::~MenuSettings()
{
}


bool MenuSettings::Update()
{
    bool result=false;

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
        {
            switch(mCurrentOption)
            {
                case 0: // Game Mode
                {
                    mSettings->mGameMode=(mSettings->mGameMode+1)%2;
                    switch(mSettings->mGameMode)
                    {
                        case 0: strcpy(mOptionNames[0], "GAME MODE: NORMAL"); break;
                        case 1: strcpy(mOptionNames[0], "GAME MODE: PRACTISE"); break;
                    }
                    mUpdated = true;
                    break;
                }
                case 1: // Control Mode
                {
                    mSettings->mControlMode=(mSettings->mControlMode+1)%3;
                    switch(mSettings->mControlMode)
                    {
                        case 0: strcpy(mOptionNames[1], "CONTROL MODE: BUTTONS"); break;
                        case 1: strcpy(mOptionNames[1], "CONTROL MODE: STICK"); break;
                        case 2: strcpy(mOptionNames[1], "CONTROL MODE: COMBINED"); break;
                    }
                    mUpdated = true;
                    break;
                }
                case 2: // Speed
                {
                    mSettings->mSpeed=(mSettings->mSpeed+1)%3;
                    switch(mSettings->mSpeed)
                    {
                        case 0: strcpy(mOptionNames[2], "SPEED: SLOW"); break;
                        case 1: strcpy(mOptionNames[2], "SPEED: NORMAL"); break;
                        case 2: strcpy(mOptionNames[2], "SPEED: FAST"); break;
                    }
                    mUpdated = true;
                    break;
                }
                case 3: // Theme
                {
                    mThemeManager->NextTheme();
                    mSettings->mTheme=mThemeManager->GetCurrentTheme();
                    sprintf(mOptionNames[3], "THEME: %s", mThemeManager->GetCurrentThemeName());
                    if (Mix_PlayingMusic()) Mix_HaltMusic();
                    Mix_PlayMusic(mThemeManager->mIntro, -1);
                    mUpdated = true;
                    break;
                }
                case 4: // (Back)
                {
                    if (mUpdated) mSettings->Write();
                    result=true;
                    break;
                }
                default: break;
            }

            mKeyDelay=mKeyInitDelay;
        }
    }

    return result;
}
