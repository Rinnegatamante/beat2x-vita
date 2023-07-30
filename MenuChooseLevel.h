#ifndef MENUCHOOSELEVEL_H
#define MENUCHOOSELEVEL_H

#include "Menu.h"

class MenuChooseLevel : public Menu
{
    public:

        MenuChooseLevel(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 lastLevel);
        virtual ~MenuChooseLevel();
};

#endif // MENUCHOOSELEVEL_H
