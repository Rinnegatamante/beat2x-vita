#include "Song.h"
#include <iostream>
#include <vector>
#include "Consts.h"


Song::Song(char* file, SDL_Surface* buttonSet, SDL_Surface* surface, Uint32 timeShow)
{
    mButtonSet = buttonSet;
    mSurface = surface;
    mTimeShow = timeShow;

    // Load audio file

    char songFile[256];
    sprintf(songFile, "%s.ogg", file);

    mMusic = Mix_LoadMUS(songFile);
    if (mMusic==NULL)
    {
        cerr << "Error reading <" << songFile << ">: " << Mix_GetError() << endl;
        exit(1);
    }

    // Load data file
    char eventFile[256];
    sprintf(eventFile, "%s.bxe", file);
    ParseEventFile(eventFile);

    mNumEvents = (Uint16)(mEventList.size());

    // Set squares for each button
    for (Uint8 i=0; i<6; i++)
    {
        mButtonRect[i].x=i*BUTTON_SIZE_X;
        mButtonRect[i].y=BUTTON_SIZE_Y<<1;
        mButtonRect[i].w=BUTTON_SIZE_X;
        mButtonRect[i].h=BUTTON_SIZE_Y;
    }

    mTopButtonsPos.y=8;
    mTopButtonsRect.y=0;
    mTopButtonsRect.w=BUTTON_SIZE_X;
    mTopButtonsRect.h=BUTTON_SIZE_Y;

    for (Uint8 i=0; i<6; i++) mTimeButtonsHighlighted[i]=0;
}


Song::~Song()
{
    Mix_FreeMusic(mMusic);
    mMusic=NULL;
}


void Song::Start()
{
    mStartTime = SDL_GetTicks();
    if (Mix_PlayMusic(mMusic, 0)==-1)
    {
        cerr << "Error playing music: " << Mix_GetError() << endl;
        exit(1);
    }
}


Sint8 Song::Update(Uint8 keysPressed)
{
    Sint8 result = 0; // 0=nothing has happened, 1=perfect, 2=great, 3=good, 4=poor, 5=missed
    Uint32 distance;

    mCurrentTime = SDL_GetTicks()-mStartTime;

    // Updates the state of IDLE, RIGHT and WRONG events if needed.

    for (Uint16 i=0; i<mNumEvents; i++)
    {
        switch(mEventList[i].GetState())
        {
            case ButtonEvent::IDLE:
            {
                // If IDLE, checks if it's time to show it.
                if ((Sint32)(mCurrentTime+mTimeShow)>=(Sint32)(mEventList[i].GetTimePress()))
                    mEventList[i].SetState(ButtonEvent::SHOWN);
                break;
            }
            case ButtonEvent::RIGHT:
            {
                if (SDL_GetTicks() - mTimeButtonsHighlighted[mEventList[i].GetButton()]>100)
                {
                    mEventList[i].SetState(ButtonEvent::DEAD);
                    mTimeButtonsHighlighted[mEventList[i].GetButton()]=0;
                }
                break;
            }
            case ButtonEvent::WRONG:
            {
                mEventList[i].SetState(ButtonEvent::DEAD);
            }
            default: break;
        }
    }

    // Finds the button the user must press, if any.

    bool found=false;
    Uint16 first=0;
    distance=0;

    while (!found && first<mNumEvents)
    {
        if (mEventList[first].GetState()==ButtonEvent::SHOWN)
        {
            distance = abs((Sint32)mCurrentTime-(Sint32)(mEventList[first].GetTimePress()));
            found = distance<(MAX_RESPONSE_DELAY<<1);
        }

        first++;
    }
    first--;

    // Button to be pressed found.

    if (found)
    {
        if (keysPressed!=0)
        {
            // Button pressed. Check if it's the right one.

            ButtonEvent::Button button = mEventList[first].GetButton();
            if ((button==ButtonEvent::A && (keysPressed&1)) || (button==ButtonEvent::B && (keysPressed&2)) ||
                (button==ButtonEvent::X && (keysPressed&4)) || (button==ButtonEvent::Y && (keysPressed&8)) ||
                (button==ButtonEvent::L && (keysPressed&16))|| (button==ButtonEvent::R && (keysPressed&32)))
            {
                if (distance<(MAX_RESPONSE_DELAY>>2))
                {
                    mEventList[first].SetState(ButtonEvent::RIGHT);
                    result=1;
                }
                else if (distance<(MAX_RESPONSE_DELAY>>1) && distance>=(MAX_RESPONSE_DELAY>>2))
                {
                    mEventList[first].SetState(ButtonEvent::RIGHT);
                    result=2;
                }
                else if (distance<((MAX_RESPONSE_DELAY*3)>>2) && distance>=(MAX_RESPONSE_DELAY>>1))
                {
                    mEventList[first].SetState(ButtonEvent::RIGHT);
                    result=3;
                }
                else if (distance>=((MAX_RESPONSE_DELAY*3)>>2))
                {
                    mEventList[first].SetState(ButtonEvent::DISABLED);
                    result=4;
                }

                mTimeButtonsHighlighted[mEventList[first].GetButton()]=SDL_GetTicks();
            }
            else
            {
                result=4;
                mEventList[first].SetState(ButtonEvent::DISABLED);
            }
        }
        else
        {
            // Button not pressed. Check if it's too late to do it.

            if (mCurrentTime>MAX_RESPONSE_DELAY+mEventList[first].GetTimePress())
            {
                mEventList[first].SetState(ButtonEvent::WRONG);
                result=5;
            }
        }
    }

    return result;
}


void Song::Draw()
{
    // Draw top buttons

    for (Uint8 i=0; i<7; i++)
    {
        mTopButtonsPos.x=i*53;
        mTopButtonsRect.x=i*BUTTON_SIZE_X;
        SDL_BlitSurface(mButtonSet, &mTopButtonsRect, mSurface, &mTopButtonsPos);
    }

    // Draw shown buttons

    SDL_Rect buttonPos;
    Uint16 yIni=240, yEnd=8;
    Uint32 percent;

    for (Sint16 i=(Sint16)mNumEvents-1; i>=0; i--)
    {
        switch(mEventList[i].GetState())
        {
            case ButtonEvent::SHOWN:
            {
                buttonPos.x=mEventList[i].GetButton()*53;

                percent = (mCurrentTime - (mEventList[i].GetTimePress()-mTimeShow))*10000;
                percent /= mTimeShow;
                buttonPos.y = (yIni*10000 + (Uint32)(yEnd-yIni)*percent)/10000;
                SDL_BlitSurface(mButtonSet, &(mButtonRect[mEventList[i].GetButton()]), mSurface, &buttonPos);
                break;
            }
            case ButtonEvent::DISABLED:
            {
                if (mCurrentTime<=mEventList[i].GetTimePress()+MAX_RESPONSE_DELAY)
                {
                    buttonPos.x=mEventList[i].GetButton()*53;

                    percent = (mCurrentTime - (mEventList[i].GetTimePress()-mTimeShow))*10000;
                    percent /= mTimeShow;
                    buttonPos.y = (yIni*10000 + (Uint32)(yEnd-yIni)*percent)/10000;
                    SDL_BlitSurface(mButtonSet, &(mButtonRect[mEventList[i].GetButton()]), mSurface, &buttonPos);
                }
                break;
            }
            case ButtonEvent::RIGHT:
            {
                SDL_Rect buttonRect;
                buttonRect.x=mEventList[i].GetButton()*BUTTON_SIZE_X;
                buttonRect.y=BUTTON_SIZE_Y;
                buttonRect.w=BUTTON_SIZE_X;
                buttonRect.h=BUTTON_SIZE_Y;
                buttonPos.x=mEventList[i].GetButton()*53;
                buttonPos.y=8;
                SDL_BlitSurface(mButtonSet, &buttonRect, mSurface, &buttonPos);
                break;
            }
            case ButtonEvent::DEAD:
            {
                mButtonRect[mEventList[i].GetButton()].y=BUTTON_SIZE_Y<<1;
                break;
            }
            default: break;
        }
    }
}


void Song::ParseEventFile(char* file)
{
    vector<Definition> definitions;
    ButtonEvent buttonEvent;

    FILE* hFile = fopen(file, "rt");
    if (hFile == NULL)
    {
        cerr << "Error reading <" << file << ">: " << SDL_GetError() << endl;
        exit(1);
    }

    char line[256], *item;
    while (fgets(line, 256, hFile)!=NULL)
    {
        if (strlen(line)>1 && line[0]!='#')
        {
            if ((line[0]=='d') && (line[1]=='e') && (line[2]=='f'))
            {
                // 'def' found

                // Get id
                item = strtok(line, " \t");
                item = strtok(NULL, " \t");

                bool found=false;
                for (Uint8 i=0; i<definitions.size() && !found; i++)
                    found = (definitions[i].id == atoi(item));

                if (found)
                {
                    cerr << "Wrong file format <" << file << ">: " << atoi(item) << " already defined." << endl;
                    exit(1);
                }

                Definition def;
                def.id = atoi(item);

                // Get events
                while ((line[0]!='e') && (line[1]!='n') && (line[2]!='d'))
                {
                    if (fgets(line, 256, hFile)==NULL)
                    {
                        cerr << "Wrong file format <" << file << ">" << endl;
                        exit(1);
                    }

                    if ((line[0]!='e') && (line[1]!='n') && (line[2]!='d'))
                    {
                        if (strlen(line)>1 && line[0]!='#')
                        {
                            // Read button
                            item = strtok(line, " \t");
                            switch(item[0])
                            {
                                case 'L': buttonEvent.SetButton(ButtonEvent::L); break;
                                case 'R': buttonEvent.SetButton(ButtonEvent::R); break;
                                case 'A': buttonEvent.SetButton(ButtonEvent::A); break;
                                case 'B': buttonEvent.SetButton(ButtonEvent::B); break;
                                case 'X': buttonEvent.SetButton(ButtonEvent::X); break;
                                case 'Y': buttonEvent.SetButton(ButtonEvent::Y); break;
                                default: break;
                            }

                            // Read when to press
                            item = strtok(NULL, " \t");
                            buttonEvent.SetTimePress((Uint32)atoi(item)+SOUND_DELAY);
                            def.button.push_back(buttonEvent);
                            mEventList.push_back(buttonEvent);
                        }
                    }
                }
                definitions.push_back(def);
            }
            else if ((line[0]=='u') && (line[1]=='s') && (line[2]=='e'))
            {
                // 'use' found

                item = strtok(line, " \t");
                item = strtok(NULL, " \t");
                Uint8 id = atoi(item);
                item = strtok(NULL, " \t");
                Uint32 millisecond=atoi(item)+SOUND_DELAY;

                Uint8 defined=0;
                bool found=false;
                for (Uint8 i=0; i<definitions.size() && !found; i++)
                    if (found = (definitions[i].id==id))
                        defined=i;

                if (!found)
                {
                    cerr << "Wrong file format <" << file << ">: " << (int)id << " undefined." << endl;
                    exit(1);
                }

                buttonEvent.SetButton(definitions[defined].button[0].GetButton());
                buttonEvent.SetTimePress(millisecond);
                mEventList.push_back(buttonEvent);

                Uint32 distance=millisecond-definitions[defined].button[0].GetTimePress();
                for (Uint8 i=1; i<definitions[defined].button.size(); i++)
                {
                    buttonEvent.SetButton(definitions[defined].button[i].GetButton());
                    buttonEvent.SetTimePress(definitions[defined].button[i].GetTimePress()+distance);
                    mEventList.push_back(buttonEvent);
                }
            }
            else
            {
                // Read button
                item = strtok(line, " \t");
                switch(item[0])
                {
                    case 'L': buttonEvent.SetButton(ButtonEvent::L); break;
                    case 'R': buttonEvent.SetButton(ButtonEvent::R); break;
                    case 'A': buttonEvent.SetButton(ButtonEvent::A); break;
                    case 'B': buttonEvent.SetButton(ButtonEvent::B); break;
                    case 'X': buttonEvent.SetButton(ButtonEvent::X); break;
                    case 'Y': buttonEvent.SetButton(ButtonEvent::Y); break;
                    default: break;
                }

                // Read when to press
                item = strtok(NULL, " \t");
                buttonEvent.SetTimePress((Uint32)atoi(item)+SOUND_DELAY);
                mEventList.push_back(buttonEvent);
            }
        }
    }

    fclose(hFile);
}
