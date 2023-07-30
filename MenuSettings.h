#ifndef MENUSETTINGS_H
#define MENUSETTINGS_H

#include "Menu.h"
#include "Settings.h"


class MenuSettings : public Menu
{
    public:

        MenuSettings(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Settings* settings);
        virtual ~MenuSettings();

        bool Update();

    private:

        Settings* mSettings;
        bool mUpdated;
};

#endif // MENUSETTINGS_H
