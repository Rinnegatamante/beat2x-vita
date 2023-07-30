#ifndef MENUHISCORES_H
#define MENUHISCORES_H

#include "Menu.h"
#include "ScoresManager.h"


class MenuHiscores : public Menu
{
    public:

        MenuHiscores(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, ScoresManager* scoresManager, Uint8 currentPack);
        ~MenuHiscores();

        void Draw();
        bool Update();

        inline Uint8 GetCurrentPack() { return mCurrentPack; };

    private:

        ScoresManager* mScoresManager;
        Uint8 mCurrentPack;
};

#endif // MENUHISCORES_H
