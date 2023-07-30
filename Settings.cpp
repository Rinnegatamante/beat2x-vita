#include "Settings.h"
#include <iostream>


Settings::Settings()
{
    // Set default settings

    mSpeed=1;
    mTheme=0;
    mGameMode=0;
    mControlMode=0;
}


Settings::~Settings()
{
}


void Settings::Read()
{
    FILE* settingsFile=fopen("data/settings.dat", "rb");
    if (settingsFile==NULL)
        Write();
    else
    {
        fread(&mSpeed, sizeof(Uint8), 1, settingsFile);
        fread(&mTheme, sizeof(Uint8), 1, settingsFile);
        fread(&mGameMode, sizeof(Uint8), 1, settingsFile);
        fread(&mControlMode, sizeof(Uint8), 1, settingsFile);
        fclose(settingsFile);
    }
}


void Settings::Write()
{
    FILE* settingsFile=fopen("data/settings.dat", "wb");
    fwrite(&mSpeed, sizeof(Uint8), 1, settingsFile);
    fwrite(&mTheme, sizeof(Uint8), 1, settingsFile);
    fwrite(&mGameMode, sizeof(Uint8), 1, settingsFile);
    fwrite(&mControlMode, sizeof(Uint8), 1, settingsFile);
    fclose(settingsFile);
#ifdef TARGET_GP2X
    sync();
#endif
}
