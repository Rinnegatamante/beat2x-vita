#ifndef SCORESMANAGER_H
#define SCORESMANAGER_H

#include "SDL.h"
#include <vector>

using namespace std;


class ScoresManager
{
    public:

        ScoresManager();
        virtual ~ScoresManager();

        void CleanUpFile();
        void ReadFile();
        void WriteFile();

        bool IsAHiScore(Uint16 score, Uint8 pack);
        void AddScore(char* initials, Uint16 score, Uint8 level, Uint8 pack);

        class Row
        {
            public:
                char initials[4];
                Uint16 score;
                Uint8 level;
        };

        class Pack
        {
            public:
                Uint16 lengthName;
                char packName[256];
                vector<Row> rows;
        };

        Uint8 GetNumPacks() { return mScores.size(); };
        char* GetPackName(Uint8 pack) { return mScores[pack].packName; };
        Row* GetRow(Uint8 pack, Uint8 row) { return &(mScores[pack].rows[row]); };

    private:

        vector<Pack> mScores;
        bool mRead;
};


#endif // SCORESMANAGER_H
