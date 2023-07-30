#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "SDL.h"
#include "usbjoy.h"

class InputHandler
{
    public:

        InputHandler();
        ~InputHandler();

        void Update();

        void KeyDown(const Uint8& key);
        void KeyDown(const SDLKey& key);
        void KeyUp(const SDLKey& key);
        void KeyUp(const Uint8& key);
		void USBUpdate();

        inline bool IsStart()       { return mKStart; };
        inline bool IsSelect()      { return mKSelect; };
        inline bool IsUp()          { return mKUp; };
        inline bool IsDown()        { return mKDown; };
        inline bool IsLeft()        { return mKLeft; };
        inline bool IsRight()       { return mKRight; };
        inline bool IsUpLeft()      { return mKUpLeft; };
        inline bool IsUpRight()     { return mKUpRight; };
        inline bool IsDownLeft()    { return mKDownLeft; };
        inline bool IsDownRight()   { return mKDownRight; };
        inline bool IsA()           { return mKA; };
        inline bool IsB()           { return mKB; };
        inline bool IsX()           { return mKX; };
        inline bool IsY()           { return mKY; };
        inline bool IsL()           { return mKL; };
        inline bool IsR()           { return mKR; };
        inline bool IsVolumeUp()    { return mKVolumeUp; };
        inline bool IsVolumeDown()  { return mKVolumeDown; };

        void ResetKeys();

    private:

        SDL_Event mEvent;
		usbjoy * joy;
        bool mKStart, mKSelect, mKUp, mKDown, mKLeft, mKRight, mKUpLeft, mKUpRight, mKDownLeft, mKDownRight, mKA, mKB, mKX, mKY, mKL, mKR, mKVolumeUp, mKVolumeDown;
};

#endif // INPUTHANDLER_H
