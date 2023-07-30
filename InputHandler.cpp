#include "InputHandler.h"
#include "Consts.h"

InputHandler::InputHandler()
{
    ResetKeys();
    joy = joy_open(1);
}


InputHandler::~InputHandler()
{
  joy_close(joy);
}


void InputHandler::Update()
{
    if (SDL_PollEvent(&mEvent))
    {
        switch(mEvent.type)
        {
#if defined(TARGET_GP2X) || defined(__vita__)
            case SDL_JOYBUTTONDOWN: KeyDown(mEvent.jbutton.button); break;
            case SDL_JOYBUTTONUP: KeyUp(mEvent.jbutton.button); break;
#else
            case SDL_KEYDOWN: KeyDown(mEvent.key.keysym.sym); break;
            case SDL_KEYUP: KeyUp(mEvent.key.keysym.sym); break;
#endif
        }
    }
	if (joy_update (joy) > 0)
	{
	    USBUpdate();
	}
}


#if defined(TARGET_GP2X) || defined(__vita__)
void InputHandler::KeyDown(const Uint8& key)
#else
void InputHandler::KeyDown(const SDLKey& key)
#endif
{
    switch(key)
    {
        case GP2X_BUTTON_START:     mKStart=true; break;
        case GP2X_BUTTON_SELECT:    mKSelect=true; break;
        case GP2X_BUTTON_UP:        mKUp=true; break;
        case GP2X_BUTTON_DOWN:      mKDown=true; break;
        case GP2X_BUTTON_LEFT:      mKLeft=true; break;
        case GP2X_BUTTON_RIGHT:     mKRight=true; break;
        case GP2X_BUTTON_UPLEFT:    mKUpLeft=true; break;
        case GP2X_BUTTON_UPRIGHT:   mKUpRight=true; break;
        case GP2X_BUTTON_DOWNLEFT:  mKDownLeft=true; break;
        case GP2X_BUTTON_DOWNRIGHT: mKDownRight=true; break;
        case GP2X_BUTTON_A:         mKA=true; break;
        case GP2X_BUTTON_B:         mKB=true; break;
        case GP2X_BUTTON_X:         mKX=true; break;
        case GP2X_BUTTON_Y:         mKY=true; break;
        case GP2X_BUTTON_L:         mKL=true; break;
        case GP2X_BUTTON_R:         mKR=true; break;
        case GP2X_BUTTON_VOLUP:     mKVolumeUp=true; break;
        case GP2X_BUTTON_VOLDOWN:   mKVolumeDown=true; break;
        default: break;
    }
}


#if defined(TARGET_GP2X) || defined(__vita__)
void InputHandler::KeyUp(const Uint8& key)
#else
void InputHandler::KeyUp(const SDLKey& key)
#endif
{
    switch(key)
    {
        case GP2X_BUTTON_START:     mKStart=false; break;
        case GP2X_BUTTON_SELECT:    mKSelect=false; break;
        case GP2X_BUTTON_UP:        mKUp=false; break;
        case GP2X_BUTTON_DOWN:      mKDown=false; break;
        case GP2X_BUTTON_LEFT:      mKLeft=false; break;
        case GP2X_BUTTON_RIGHT:     mKRight=false; break;
        case GP2X_BUTTON_UPLEFT:    mKUpLeft=false; break;
        case GP2X_BUTTON_UPRIGHT:   mKUpRight=false; break;
        case GP2X_BUTTON_DOWNLEFT:  mKDownLeft=false; break;
        case GP2X_BUTTON_DOWNRIGHT: mKDownRight=false; break;
        case GP2X_BUTTON_A:         mKA=false; break;
        case GP2X_BUTTON_B:         mKB=false; break;
        case GP2X_BUTTON_X:         mKX=false; break;
        case GP2X_BUTTON_Y:         mKY=false; break;
        case GP2X_BUTTON_L:         mKL=false; break;
        case GP2X_BUTTON_R:         mKR=false; break;
        case GP2X_BUTTON_VOLUP:     mKVolumeUp=false; break;
        case GP2X_BUTTON_VOLDOWN:   mKVolumeDown=false; break;
        default: break;
    }
}

void InputHandler::USBUpdate()
{
  mKUp  = joy_getbutton(2, joy) || joy_getaxe (JOYUP, joy);
  mKDown  = joy_getbutton(1, joy) || joy_getaxe (JOYDOWN, joy);
  mKLeft  = joy_getbutton(0, joy) || joy_getaxe (JOYLEFT, joy);
  mKRight  = joy_getbutton(3, joy) || joy_getaxe (JOYRIGHT, joy);
  mKL = joy_getbutton(4, joy) || joy_getbutton(6, joy) || 
    (joy_getaxe (JOYUP, joy) && joy_getaxe (JOYLEFT, joy));
  mKR = joy_getbutton(7, joy) || joy_getbutton(5, joy) ||
  (joy_getaxe (JOYUP, joy) && joy_getaxe (JOYRIGHT, joy));
  mKB = joy_getbutton(8, joy);
}


void InputHandler::ResetKeys()
{
    SDL_Event event;
    while(SDL_PollEvent(&event));
    mKStart=mKSelect=mKUp=mKDown=mKLeft=mKRight=mKUpLeft=mKUpRight=mKDownLeft=mKDownRight=mKA=mKB=mKX=mKY=mKL=mKR=mKVolumeUp=mKVolumeDown=false;
}
