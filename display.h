#pragma once

#include <iostream>
#include <Windows.h>
#include <memory>
#include <string>

namespace Cinnamon {

    struct Screen {
        HANDLE                          hOut;
        CONSOLE_SCREEN_BUFFER_INFO      csbi;
        DWORD                           cellCount;
        COORD                           homeCoords = { 0, 0 };
        COORD                           size;
        CONSOLE_CURSOR_INFO             cursorInfo;
        std::unique_ptr<const wchar_t>  buffer;

        Screen(HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE),
            COORD size = { 120, 30 })
            : hOut(hOut),
            size(size)
        {
            GetConsoleScreenBufferInfo(hOut, &csbi);
            SetConsoleScreenBufferSize(hOut, COORD{ 120, 30 });
            cellCount = csbi.dwSize.X * csbi.dwSize.Y;
            GetConsoleCursorInfo(this->hOut, &cursorInfo);
        }

    };


    void RenderScreen(Screen& screen)
    {
        size_t textlen = wcslen(screen.buffer.get());
        DWORD* txtlenout = new DWORD{ 0 };
        WriteConsoleOutputCharacterW(screen.hOut, screen.buffer.get(), (DWORD)textlen, screen.homeCoords, txtlenout);
        delete txtlenout;
    }

    void DebugLog(const wchar_t msg)
    {
        OutputDebugString(&msg);
    }

    void DebugLog(const wchar_t* msg)
    {
        OutputDebugString(msg);
    }

    void DebugLog(int msg)
    {
        DebugLog(std::to_wstring(msg).c_str());
    }

}
