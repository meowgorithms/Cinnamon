#pragma once
#include <iostream>
#include <Windows.h>
#include "Screen.h"
#include "utils.h"
#include "Debug.h"
#include "CinnamonObject.h"

// TODO: fix mouse button input
namespace Cinnamon {
    static class Input {
    public:
        inline static DWORD eventsRead;
        inline static INPUT_RECORD irec;

        inline static bool KeyDown(int KeyCode) {
            if (KeyCode == VK_LBUTTON) {
                return leftMousePressed;
            }

            if (KeyCode == VK_RBUTTON) {
                return rightMousePressed;
            }

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
        
        inline static void Update() {
            leftMouseDown = false;
            rightMouseDown = false;

            // Left mouse down
            if ((GetKeyState(VK_LBUTTON) & 0x8000) && !leftMousePressed) {
                leftMousePressed = true;
                leftMouseDown = true;
                leftMouseUp = false;
            }
            
            // Left mouse up
            if (!(GetKeyState(VK_LBUTTON) & 0x8000) && leftMousePressed) {
                leftMousePressed = false;
                leftMouseDown = false;
                leftMouseUp = true;
            }

            // Right mouse down
            if ((GetKeyState(VK_RBUTTON) & 0x8000) && !rightMousePressed) {
                rightMouseDown = true;
                rightMouseUp = false;
                rightMousePressed = true;
            }
            
            // Right mouse up
			if (!(GetKeyState(VK_RBUTTON) & 0x8000) && rightMousePressed) {
                rightMouseUp = true;
                rightMouseDown = false;
                rightMousePressed = false;
            }
        }

        // Did the left mouse button transition from up to down this frame
        inline static bool GetleftMouseDown() {
            return leftMouseDown;
        }

        // Is the left mouse button down this frame
        inline static bool GetLeftMousePressed() {
            return leftMousePressed;
        }

        // Did the left mouse button transition from down to up this frame
        inline static bool GetLeftMouseUp() {
            return leftMouseUp;
        }

		// Did the right mouse button transition from up to down this frame
        inline static bool GetRightMouseDown() {
            return rightMouseDown;
        }

        inline static bool GetRightMousePressed() {
            return rightMousePressed;
        }

        inline static bool GetRightMouseUp() {
            return rightMouseUp;
        }

    protected:
        inline static bool leftMouseDown = false;
        inline static bool leftMouseUp = true;
        inline static bool leftMousePressed = false;

        inline static bool rightMouseDown = false;
        inline static bool rightMouseUp = true;
        inline static bool rightMousePressed = false;

    private:
        inline static COORD prevMousePos;
        inline static COORD mousePos;
    };
}

        // Gotta use VK KeyState in order to not drop inputs,
        // Gotta use irec to get the mouse coordinates in screen space
        // & 0x8000 just checks that the button is down, mouseDown keeps it from spamming
