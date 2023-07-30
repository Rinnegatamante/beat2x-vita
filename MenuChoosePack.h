#ifndef MENUCHOOSEPACK_H
#define MENUCHOOSEPACK_H

#include "Menu.h"


class MenuChoosePack : public Menu
{
    public:
        MenuChoosePack(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 currentPack);
        virtual ~MenuChoosePack();

        const char* GetCurrentPackFolder() { return mOptionNames[GetOption()]; };
        inline Uint8 GetCurrentPack() { return GetOption(); };
        inline Uint8 GetLastPlayedSong() { return mLastPlayedSong[GetOption()]; };
        inline Uint8 GetNumSongs() { return mNumSongs[GetOption()]; };

    private:

        vector<Uint8> mLastPlayedSong;  // Last played song for each pack
        vector<Uint8> mNumSongs;        // Number of songs for each pack
};

#endif // MENUCHOOSEPACK_H
