#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"


class MenuMain : public Menu
{
    public:
        MenuMain(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 controlMode, Uint8 gameMode);
        virtual ~MenuMain();
};

#endif // MAINMENU_H
