#include "ScoresManager.h"

#ifdef TARGET_WIN32
#   include <windows.h>
#endif
#include <string.h>
#include <stdio.h>
#include <iostream>

#ifndef TARGET_WIN32
#   include <unistd.h>
extern int alphasort();
#   include <dirent.h>
#endif


ScoresManager::ScoresManager()
{
    mRead=false;
}


ScoresManager::~ScoresManager()
{
}


void ScoresManager::CleanUpFile()
{
    // Read pack folders

    vector<char*> packs;

    Uint8 numPacks=0;

#ifndef TARGET_WIN32
    struct dirent **list;
    int n=scandir("packs", &list, NULL, alphasort);

    for (Uint8 i=0; i<n; i++)
    {
        if ((strcmp(list[i]->d_name, ".")!=0) && (strcmp(list[i]->d_name, "..")!=0) && (strcmp(list[i]->d_name, "do_not_delete.txt")!=0))
        {
            packs.push_back(new char[strlen(list[i]->d_name)+1]);
            strcpy(packs[numPacks], list[i]->d_name);
            numPacks++;
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
            packs.push_back(new char[strlen(data.cFileName)+1]);
            strcpy(packs[numPacks], data.cFileName);
            numPacks++;
        }
        while (FindNextFile(folders, &data)!=0)
        {
            if ((strcmp(data.cFileName, ".")!=0) && (strcmp(data.cFileName, "..")!=0) && (strcmp(data.cFileName, "do_not_delete.txt")!=0))
            {
                packs.push_back(new char[strlen(data.cFileName)+1]);
                strcpy(packs[numPacks], data.cFileName);
                for (Uint8 i=0; i<strlen(packs[numPacks]); i++)
                    packs[numPacks][i]=toupper(packs[numPacks][i]);
                numPacks++;
            }
        }
        FindClose(folders);
    }
#endif

    char initials[4];
    strcpy(initials, "AAA");
    Uint16 score=0;
    Uint8 level=1;
    Uint16 length;

    FILE* scoresFile = fopen("data/scores.dat", "rb");

    if (scoresFile==NULL)
    {
        // If the file doesn't exists, it creates one with data for each
        // available pack.

        scoresFile = fopen("data/scores.dat", "wb");

        for (Uint8 i=0; i<numPacks; i++)
        {
            length=strlen(packs[i]);
            fwrite(&length, sizeof(Uint16), 1, scoresFile);                     // Pack name length
            fwrite(packs[i], sizeof(char)*(length+1), 1, scoresFile);           // Pack name
            for (Uint8 j=0; j<10; j++)
            {
                fwrite(initials, sizeof(char)*4, 1, scoresFile);                // Initials
                fwrite(&score, sizeof(Uint16), 1, scoresFile);                 // Score
                fwrite(&level, sizeof(Uint8), 1, scoresFile);                  // Level
            }
        }

        fclose(scoresFile);
#ifdef TARGET_GP2X
        sync();
#endif
    }
    else
    {
        // If the file exists, check if the file data is coherent
        // with the packs folder.

        // Retrieve packs data from 'scores.dat' and copy them to packsInFile.

        vector<Pack> packsInFile;

        while(1)
        {
            Pack onePack;

            fread(&(onePack.lengthName), sizeof(Uint16), 1, scoresFile);                      // Pack name length
            if (feof(scoresFile)) break;

            fread(onePack.packName, sizeof(char)*(onePack.lengthName+1), 1, scoresFile);                // Pack name

            for (Uint8 j=0; j<10; j++)
            {
                Row oneRow;

                fread(oneRow.initials, sizeof(char)*4, 1, scoresFile);                 // Initials
                fread(&(oneRow.score), sizeof(Uint16), 1, scoresFile);                  // Score
                fread(&(oneRow.level), sizeof(Uint8), 1, scoresFile);                   // Level

                onePack.rows.push_back(oneRow);
            }

            packsInFile.push_back(onePack);
        }

        fclose(scoresFile);

        // Overwrite 'scores.dat' with the new data

        scoresFile = fopen("data/scores.dat", "wb");

        bool found;
        Uint8 j;
        for (Uint8 i=0; i<numPacks; i++)
        {
            found=false;

            for (j=0; j<packsInFile.size() && !found; j++)
                found = (strcmp(packs[i], packsInFile[j].packName)==0);

            if (found)
            {
                // Found, copy it.

                j--;

                length=strlen(packsInFile[j].packName);
                fwrite(&length, sizeof(Uint16), 1, scoresFile);                     // Pack name length
                fwrite(packsInFile[j].packName, sizeof(char)*(length+1), 1, scoresFile);           // Pack name
                for (Uint8 k=0; k<10; k++)
                {
                    fwrite(packsInFile[j].rows[k].initials, sizeof(char)*4, 1, scoresFile);                // Initials
                    fwrite(&(packsInFile[j].rows[k].score), sizeof(Uint16), 1, scoresFile);                 // Score
                    fwrite(&(packsInFile[j].rows[k].level), sizeof(Uint8), 1, scoresFile);                  // Level
                }
            }
            else
            {
                // Not found, set default values.

                length=strlen(packs[i]);
                fwrite(&length, sizeof(Uint16), 1, scoresFile);                     // Pack name length
                fwrite(packs[i], sizeof(char)*(length+1), 1, scoresFile);           // Pack name
                for (Uint8 j=0; j<10; j++)
                {
                    fwrite(initials, sizeof(char)*4, 1, scoresFile);                // Initials
                    fwrite(&score, sizeof(Uint16), 1, scoresFile);                 // Score
                    fwrite(&level, sizeof(Uint8), 1, scoresFile);                  // Level
                }
            }
        }

        fclose(scoresFile);
#ifdef TARGET_GP2X
        sync();
#endif
    }
}


void ScoresManager::ReadFile()
{
    FILE* scoresFile = fopen("data/scores.dat", "rb");

    mScores.clear();

    while(1)
    {
        Pack onePack;

        fread(&(onePack.lengthName), sizeof(Uint16), 1, scoresFile);                      // Pack name length
        if (feof(scoresFile)) break;

        fread(onePack.packName, sizeof(char)*(onePack.lengthName+1), 1, scoresFile);                // Pack name

        for (Uint8 j=0; j<10; j++)
        {
            Row oneRow;
            fread(oneRow.initials, sizeof(char)*4, 1, scoresFile);                 // Initials
            fread(&(oneRow.score), sizeof(Uint16), 1, scoresFile);                  // Score
            fread(&(oneRow.level), sizeof(Uint8), 1, scoresFile);                   // Level

            onePack.rows.push_back(oneRow);
        }

        mScores.push_back(onePack);
    }

    fclose(scoresFile);

    mRead=true;
}


void ScoresManager::WriteFile()
{
    FILE* scoresFile = fopen("data/scores.dat", "wb");

    for (Uint8 i=0; i<mScores.size(); i++)
    {
        fwrite(&(mScores[i].lengthName), sizeof(Uint16), 1, scoresFile);                      // Pack name length
        fwrite(mScores[i].packName, sizeof(char)*(mScores[i].lengthName+1), 1, scoresFile);                // Pack name

        for (Uint8 j=0; j<10; j++)
        {
            fwrite(mScores[i].rows[j].initials, sizeof(char)*4, 1, scoresFile);                 // Initials
            fwrite(&(mScores[i].rows[j].score), sizeof(Uint16), 1, scoresFile);                  // Score
            fwrite(&(mScores[i].rows[j].level), sizeof(Uint8), 1, scoresFile);                   // Level
        }
    }

    fclose(scoresFile);

#ifdef TARGET_GP2X
    sync();
#endif
}


bool ScoresManager::IsAHiScore(Uint16 score, Uint8 pack)
{
    bool found=false;
    if (!mRead) ReadFile();

    for (Uint8 i=0; i<10 && !found; i++)
        found = score>=mScores[pack].rows[i].score;

    return found;
}


void ScoresManager::AddScore(char* initials, Uint16 score, Uint8 level, Uint8 pack)
{
    Sint8 i;
    bool found=false;

    for (i=0; i<10 && !found; i++)
        found = score>=mScores[pack].rows[i].score;

    if (found)
    {
        i--;
        Uint8 j;
        for (j=9; j>i; j--)
        {
            strcpy(mScores[pack].rows[j].initials, mScores[pack].rows[j-1].initials);
            mScores[pack].rows[j].score = mScores[pack].rows[j-1].score;
            mScores[pack].rows[j].level = mScores[pack].rows[j-1].level;
        }

        strcpy(mScores[pack].rows[i].initials, initials);

        mScores[pack].rows[i].score = score;
        mScores[pack].rows[i].level = level;
    }

    WriteFile();
    mRead=false;
}
