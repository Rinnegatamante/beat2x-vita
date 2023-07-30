#ifndef MENUENTERINITIALS_H
#define MENUENTERINITIALS_H

#include "Menu.h"


class MenuEnterInitials : public Menu
{
    public:

        MenuEnterInitials(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager);
        ~MenuEnterInitials();

        char* GetInitials() { return mInitials; };

        void Draw();
        bool Update();

    private:

        Uint8 mInitialPos;
        Uint8 mCursorPosX, mCursorPosY;
        char mCharArray[4][11];
        char mInitials[5];
        Uint32 mTimerCursor;
};

#endif // MENUENTERINITIALS_H
