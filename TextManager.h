#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "SDL.h"
#include <list>
#include "ThemeManager.h"


using namespace std;

const Uint8 CHAR_SIZE = 8;
const int LEFT    = 0x10000000;
const int RIGHT   = 0x01000000;
const int HCENTRE = 0x00100000;
const int UP      = 0x00010000;
const int DOWN    = 0x00001000;
const int VCENTRE = 0x00000100;


class TextManager
{
	public:

        TextManager(ThemeManager* themeManager, SDL_Surface* surface);
		~TextManager();
		void Write(Sint16 x, Sint16 y, char* text, int flags=LEFT|UP, bool alt=false);
		void AddTextElement(Sint16 xfrom, Sint16 yfrom, Sint16 xto, Sint16 yto, char* text, Uint32 time, int flags=LEFT|UP);
		void Update();
		void Draw();

	private:

        class TextElement
        {
            public:
                Sint16 mX;
                Sint16 mY;
                Sint16 mXFrom;
                Sint16 mYFrom;
                Sint16 mXTo;
                Sint16 mYTo;
                Uint32 mStartTime;
                Uint32 mTotalTime;
                int mFlags;
                char* mText;
        };

        ThemeManager* mThemeManager;
        SDL_Surface* mSurface;
        SDL_Rect mChar, mCharInImage;
        Sint16 mX, mY;
        Uint8 mLength;
        list<TextElement> mTextList;
};


#endif // TEXTMANAGER_H
