#pragma once
#include <iostream>
#include <Windows.h>
#include "Screen.h"
#include "utils.h"
#include "Debug.h"


namespace Cinnamon {
	
    static class Input {
    public:
        inline static DWORD eventsRead;
        inline static INPUT_RECORD irec;
        
        inline static bool KeyDown(int KeyCode) {
            if (GetKeyState(KeyCode) & 0x8000)
                return true;
            return false;
        }

        inline static COORD GetMousePosition(Screen& screen) {
            if (irec.Event.MouseEvent.dwEventFlags == 1) {
                mousePos = irec.Event.MouseEvent.dwMousePosition;
                prevMousePos = mousePos;
            }
            else
                mousePos = prevMousePos;
            return mousePos;
        }

        inline static void PeekInput(Screen& screen) {
            PeekConsoleInput(screen.hIn, &irec, 1, &eventsRead);
        }

    protected:
        inline static bool leftMouseDown = false;
        inline static bool leftMouseUp = false;
        inline static bool leftMousePressed = false;
        inline static bool rightMouseDown = false;
        inline static bool rightMouseUp = false;
        inline static bool rightMousePressed = false;

    private:
        inline static COORD prevMousePos;
        inline static COORD mousePos;
    };
}



/*
void HandleInput(double deltaTime)
    {
        // Gotta use VK KeyState in order to not drop inputs,
        // Gotta use irec to get the mouse coordinates in screen space
        // & 0x8000 just checks that the button is down, mouseDown keeps it from spamming
        COORD worldMousePos = GetWorldMousePosition();
        if ((GetKeyState(VK_LBUTTON) & 0x8000) && !mouseDown)
        {
            mouseDown = true;

            bool outOfBounds = worldMap->OutOfBounds(worldMousePos);
            WorldObject* wo;
            IAmInteractable* interactable;


            if (!outOfBounds)
            {
                wo = (worldMap->map[worldMap->CoordToIndex(worldMousePos)]);
                interactable = dynamic_cast<IAmInteractable*>(wo);

                if (interactable != nullptr && PositionInCharacterProximity(worldMousePos))
                {
                    interactable->OnInteract();
                }
            }
        }

	    if ((GetKeyState(VK_RBUTTON) & 0x8000) && !rMouseDown)
        {
           rMouseDown = true;

           bool outOfBounds = worldMap->OutOfBounds(worldMousePos);
           
        }

        if (!(GetKeyState(VK_LBUTTON) & 0x8000) && mouseDown)
            mouseDown = false;
        if (!(GetKeyState(VK_RBUTTON) & 0x8000) && rMouseDown)
            rMouseDown = false;
        
        double newPos[2]{ character->GetPosition()[0], character->GetPosition()[1] };
        // UP W
        if (GetKeyState('W') & 0x8000)
        {
            newPos[1] = newPos[1] - character->velocity * deltaTime;
        }
        // DOWN S
        if (GetKeyState('S') & 0x8000)
        {
            newPos[1] = newPos[1] + character->velocity * deltaTime;
        }
        // RIGHT D
        if (GetKeyState('D') & 0x8000)
        {
            newPos[0] = newPos[0] + character->velocity * deltaTime;
        }
        // LEFT A
        if (GetKeyState('A') & 0x8000)
        {
            newPos[0] = newPos[0] - character->velocity * deltaTime;
        }
        
        COORD maybeNewWorldPos{ SHORT(floor(newPos[0])), SHORT(floor(newPos[1])) };

        bool outOfBounds = worldMap->OutOfBounds(maybeNewWorldPos);

        if (outOfBounds)
        {
            return;
        }
        else if (worldMap->map[worldMap->CoordToIndex(maybeNewWorldPos)]->bCollision)
        {
            return;
        }
        else
            character->SetPosition(newPos);

    }
*/
