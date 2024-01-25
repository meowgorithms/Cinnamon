#pragma once

#include <iostream>
#include <Windows.h>
#include <memory>
#include <string>

namespace Cinnamon {

    struct Screen {
        HANDLE                          hOut;
        HANDLE                          hIn;
        CONSOLE_SCREEN_BUFFER_INFO      csbi;
        DWORD                           cellCount;
        COORD                           homeCoords = { 0, 0 };
        COORD                           size;
        CONSOLE_CURSOR_INFO             cursorInfo;
        wchar_t*                         buffer;

        Screen(HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE),
               HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE),
               COORD size = { 120, 30 })
            : hOut(hOut),
              hIn(hIn),
              size(size)
        {
            GetConsoleScreenBufferInfo(hOut, &csbi);
            SetConsoleScreenBufferSize(hOut, size);
            cellCount = csbi.dwSize.X * csbi.dwSize.Y;
            buffer = new wchar_t[cellCount];
            csbi.dwMaximumWindowSize = size;
            GetConsoleCursorInfo(this->hOut, &cursorInfo);
            GetConsoleCursorInfo(hOut, &cursorInfo);
            cursorInfo.bVisible = false; // set the cursor visibility
            SetConsoleCursorInfo(hOut, &cursorInfo);

            SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
            SetConsoleMode(hIn, ~ENABLE_QUICK_EDIT_MODE);

			for (size_t i = 0; i < cellCount; i++) {
                buffer[i] = L' ';
            }
        }
        
        void PlaceAt(wchar_t& c, COORD screenPos) {
            buffer[ScreenCoordToIndex(screenPos)] = c;
        }

		int ScreenCoordToIndex(COORD pos) {
            return pos.X + (pos.Y * size.X);
        }

    };

    void RenderScreen(Screen& screen)
    {
        size_t textlen = wcslen(screen.buffer);
        DWORD* txtlenout = new DWORD { 0 };
        WriteConsoleOutputCharacterW(screen.hOut, screen.buffer, (DWORD)textlen, screen.homeCoords, txtlenout);
        delete txtlenout;
    }
}
