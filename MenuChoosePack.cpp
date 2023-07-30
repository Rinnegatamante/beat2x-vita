#include "MenuChoosePack.h"
#ifdef TARGET_WIN32
#   include <windows.h>
#endif
#include <iostream>

#ifndef TARGET_WIN32
extern int alphasort();
#include <dirent.h>
#endif

#include "Consts.h"


using namespace std;


MenuChoosePack::MenuChoosePack(InputHandler* inputHandler, SDL_Surface* surface, ThemeManager* themeManager, TextManager* textManager, Uint8 currentPack) : Menu(inputHandler, surface, themeManager, textManager)
{
    mOriginY=100;

    mTitle = new char[17];
    strcpy(mTitle, "CHOOSE SONG PACK");

    Uint8 numOptions=0;

    // Read pack folders

#ifndef TARGET_WIN32
    struct dirent **list;
    int n=scandir("packs", &list, NULL, alphasort);

    for (Uint8 i=0; i<n; i++)
    {
        if ((strcmp(list[i]->d_name, ".")!=0) && (strcmp(list[i]->d_name, "..")!=0) && (strcmp(list[i]->d_name, "do_not_delete.txt")!=0))
        {
            mOptionNames.push_back(new char[strlen(list[i]->d_name)+1]);
            strcpy(mOptionNames[numOptions], list[i]->d_name);
            numOptions++;
        }
        free(list[i]);
    }
#else
    WIN32_FIND_DATA data;
    HANDLE folders = FindFirstFile("packs/*", &data);

    if (folders==INVALID_HANDLE_VALUE)
    {
        cerr << "Error reading <packs>" << endl;
        exit(1);
    }
    else
    {
        if ((strcmp(data.cFileName, ".")!=0) && (strcmp(data.cFileName, "..")!=0) && (strcmp(data.cFileName, "do_not_delete.txt")!=0))
        {
            mOptionNames.push_back(new char[strlen(data.cFileName)+1]);
            strcpy(mOptionNames[numOptions], data.cFileName);
            numOptions++;
        }
        while (FindNextFile(folders, &data)!=0)
        {
            if ((strcmp(data.cFileName, ".")!=0) && (strcmp(data.cFileName, "..")!=0) && (strcmp(data.cFileName, "do_not_delete.txt")!=0))
            {
                mOptionNames.push_back(new char[strlen(data.cFileName)+1]);
                strcpy(mOptionNames[numOptions], data.cFileName);
                for (Uint8 i=0; i<strlen(mOptionNames[numOptions]); i++)
                    mOptionNames[numOptions][i]=toupper(mOptionNames[numOptions][i]);
                numOptions++;
            }
        }
        FindClose(folders);
    }
#endif

    mOptionNames.push_back(new char[7]);
    strcpy(mOptionNames[numOptions], "(BACK)");

    mCurrentOption = currentPack;

    // Read last played song for each pack

    FILE* file;
    char filename[256];

    for (Uint8 i=0; i<mOptionNames.size()-1; i++)
    {
        Uint8 last=0;

        sprintf(filename, "packs/%s/settings.dat", mOptionNames[i]);
        file=fopen(filename, "rb");

        if (file==NULL)
        {
            file=fopen(filename, "wb");
            fwrite(&last, sizeof(Uint8), 1, file);
        }
        else
            fread(&last, sizeof(Uint8), 1, file);

        fclose(file);

#ifdef TARGET_GP2X
        sync();
#endif

        mLastPlayedSong.push_back(last);
    }

    // Count the number of songs for each pack

    char songListFile[256];
    char line[256];
    Uint8 numSongs;
    FILE* hSongListFile;

    for (Uint8 i=0; i<mOptionNames.size()-1; i++)
    {
        numSongs = 0;
        sprintf(songListFile, "packs/%s/songlist.bxs", mOptionNames[i]);

        hSongListFile = fopen(songListFile, "rt");
        if (hSongListFile == NULL)
        {
            cerr << "Error reading <" << songListFile << ">: " << SDL_GetError() << endl;
            exit(1);
        }

        while (fgets(line, 256, hSongListFile)!=NULL)
            if (strlen(line)>1 && line[0]!='#')
                numSongs++;

        fclose(hSongListFile);

        mNumSongs.push_back(numSongs);
    }
}


MenuChoosePack::~MenuChoosePack()
{
}
