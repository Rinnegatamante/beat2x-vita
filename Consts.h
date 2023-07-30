#ifndef DEFINICIONS_H
#define DEFINICIONS_H

#include "SDL.h"

const Uint16 RESX = 320;
const Uint16 RESY = 240;
const Uint16 RESCOL = 16;
const Sint16 BUTTON_SIZE_X = 57;
const Sint16 BUTTON_SIZE_Y = 57;
const Uint8 FRAME_RATE = 10;
const Uint32 MAX_RESPONSE_DELAY = 175;
const Uint8 MAX_SONGS=10;
#ifdef TARGET_GP2X
const Uint16 SOUND_DELAY=345;
#else
const Uint16 SOUND_DELAY=120;
#endif

#ifdef __vita__
#   define GP2X_BUTTON_START           (11)
#   define GP2X_BUTTON_SELECT          (10)
#   define GP2X_BUTTON_UP              (8)
#   define GP2X_BUTTON_DOWN            (6)
#   define GP2X_BUTTON_LEFT            (7)
#   define GP2X_BUTTON_RIGHT           (9)
#   define GP2X_BUTTON_UPLEFT          (19)
#   define GP2X_BUTTON_UPRIGHT         (20)
#   define GP2X_BUTTON_DOWNLEFT        (21)
#   define GP2X_BUTTON_DOWNRIGHT       (22)
#   define GP2X_BUTTON_CLICK           (18)
#   define GP2X_BUTTON_A               (2)
#   define GP2X_BUTTON_B               (1)
#   define GP2X_BUTTON_X               (3)
#   define GP2X_BUTTON_Y               (0)
#   define GP2X_BUTTON_L               (4)
#   define GP2X_BUTTON_R               (5)
#   define GP2X_BUTTON_VOLUP           (16)
#   define GP2X_BUTTON_VOLDOWN         (17)
#elif defined(TARGET_GP2X)
#   define GP2X_BUTTON_START           (8)
#   define GP2X_BUTTON_SELECT          (9)
#   define GP2X_BUTTON_UP              (0)
#   define GP2X_BUTTON_DOWN            (4)
#   define GP2X_BUTTON_LEFT            (2)
#   define GP2X_BUTTON_RIGHT           (6)
#   define GP2X_BUTTON_UPLEFT          (1)
#   define GP2X_BUTTON_UPRIGHT         (7)
#   define GP2X_BUTTON_DOWNLEFT        (3)
#   define GP2X_BUTTON_DOWNRIGHT       (5)
#   define GP2X_BUTTON_CLICK           (18)
#   define GP2X_BUTTON_A               (12)
#   define GP2X_BUTTON_B               (13)
#   define GP2X_BUTTON_X               (15)
#   define GP2X_BUTTON_Y               (14)
#   define GP2X_BUTTON_L               (11)
#   define GP2X_BUTTON_R               (10)
#   define GP2X_BUTTON_VOLUP           (16)
#   define GP2X_BUTTON_VOLDOWN         (17)
#else
#   define GP2X_BUTTON_START           (SDLK_RETURN)
#   define GP2X_BUTTON_SELECT          (SDLK_RSHIFT)
#   define GP2X_BUTTON_UP              (SDLK_UP)
#   define GP2X_BUTTON_DOWN            (SDLK_DOWN)
#   define GP2X_BUTTON_LEFT            (SDLK_LEFT)
#   define GP2X_BUTTON_RIGHT           (SDLK_RIGHT)
#   define GP2X_BUTTON_UPLEFT          (SDLK_KP7)
#   define GP2X_BUTTON_UPRIGHT         (SDLK_KP9)
#   define GP2X_BUTTON_DOWNLEFT        (SDLK_KP1)
#   define GP2X_BUTTON_DOWNRIGHT       (SDLK_KP3)
#   define GP2X_BUTTON_A               (SDLK_j)
#   define GP2X_BUTTON_B               (SDLK_k)
#   define GP2X_BUTTON_X               (SDLK_m)
#   define GP2X_BUTTON_Y               (SDLK_i)
#   define GP2X_BUTTON_L               (SDLK_q)
#   define GP2X_BUTTON_R               (SDLK_w)
#   define GP2X_BUTTON_VOLUP           (SDLK_KP8)
#   define GP2X_BUTTON_VOLDOWN         (SDLK_KP2)
#endif


#endif // DEFINICIONS_H
