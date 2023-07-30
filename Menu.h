#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include "InputHandler.h"
#include "TextManager.h"
#include "ThemeManager.h"
#include <vector>

using namespace std;

const Uint32 BUTTON_DELAY=150;


class Menu
{
    public:

        Menu(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 defaultOption=0, bool altBackground=false);
        virtual ~Menu();

        void Draw();
        bool Update();
        inline Uint8 GetOption() { return mCurrentOption; };
        inline Uint8 GetNumOptions() { return mOptionNames.size(); };

    protected:

        void NextOption();
        void PreviousOption();

        char* mTitle;
        Uint8 mCurrentOption;
        vector<char*> mOptionNames;

        InputHandler* mInputHandler;
        TextManager* mTextManager;
        SDL_Surface* mSurface;

        ThemeManager* mThemeManager;

        Uint16 mOriginY;
        Uint8 mKeyDelay, mKeyInitDelay;

        Uint8 mMaxOptionsPerPage;

        bool mAltBackground;
};

#endif // MENU_H
