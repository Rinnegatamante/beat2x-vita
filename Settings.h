#ifndef SETTINGS_H
#define SETTINGS_H

#include "SDL.h"


class Settings
{
    public:

        Settings();
        virtual ~Settings();

        void Read();
        void Write();

        Uint8 mSpeed;
        Uint8 mTheme;
        Uint8 mGameMode;
        Uint8 mControlMode;
};

#endif // SETTINGS_H
