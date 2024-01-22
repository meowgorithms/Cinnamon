#pragma once

#include <iostream>
#include <Windows.h>


namespace Cinnamon {
	
	static class Input {
	public:

        // KeyCode -> VK codes
		inline static bool KeyDown(int KeyCode) {
            if (GetKeyState(KeyCode) & 0x8000)
                return true;
            return false;
		}

	protected:
	private:
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
