#include "gfx.h"


Uint32 GetPixel(SDL_Surface* surface, int x, int y)
{
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 2;
    return *(Uint16 *)p;
}


void SetPixel(SDL_Surface* surface, int x, int y, Uint32 colour )
{
    if(SDL_MUSTLOCK(surface))
        if(SDL_LockSurface(surface) < 0)
            return;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 2;
    *(Uint16 *)p = colour;

  if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}
