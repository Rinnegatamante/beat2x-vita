#ifndef SONG_H
#define SONG_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "ButtonEvent.h"
#include <vector>
#include <math.h>


using namespace std;


class Song
{
    public:

        Song(char* file, SDL_Surface* buttonSet, SDL_Surface* surface, Uint32 timeShow);
        ~Song();

        void Start();
        Sint8 Update(Uint8 keysPressed);
        void Draw();

        inline Uint16 GetNumEvents() { return mNumEvents; };
        bool HasEnded() { return Mix_PlayingMusic()==0; };

    private:

        Mix_Music* mMusic;

        vector<ButtonEvent> mEventList;
        Uint16 mNumEvents;
        Uint32 mStartTime;
        Uint32 mCurrentTime;
        Uint32 mTimeShow;

        SDL_Surface* mSurface;
        SDL_Surface* mButtonSet;
        SDL_Rect mButtonRect[6]; // L=0, A=1, Y=2, X=3, B=4, R=5
        Uint32 mTimeButtonsHighlighted[6];
        SDL_Rect mTopButtonsPos, mTopButtonsRect;

        void ParseEventFile(char* file);

        class Definition
        {
            public:
                Uint8 id;
                vector<ButtonEvent> button;
        };
};

#endif // SONG_H
