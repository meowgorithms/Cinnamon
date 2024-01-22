#pragma once
#include <Windows.h>
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <memory>

namespace Cinnamon {

    // Goal: Create gui module using a more 'procedural paradigm' approach

    // Constants for drawing
#define VERTICAL_BORDER     0x2551
#define HORIZONTAL_BORDER   0x2550
#define TOP_LEFT_BORDER     0x2554
#define TOP_RIGHT_BORDER    0x2557
#define BOTTOM_LEFT_BORDER  0x255A
#define BOTTOM_RIGHT_BORDER 0x255D
#define EXIT                0x2716


    class TextBase
    {
    public:
        wchar_t* display = nullptr;
        COORD size = { 5, 5 };
        COORD screenPosition = { 0, 0 };

        COORD ScreenSpaceCoordToMenuCoord(COORD screenPosition)
        {
            return { SHORT(screenPosition.X - this->screenPosition.X), SHORT(screenPosition.Y - this->screenPosition.Y) };
        }

        int LocalCoordToIndex(COORD menuPosition)
        {
            return menuPosition.X + (menuPosition.Y * size.X);
        }

        void UpdateDisplay()
        {
            // Fill corners
            display[0] = wchar_t{ TOP_LEFT_BORDER };
            display[LocalCoordToIndex({ SHORT(size.X - 1), 0 })] = wchar_t{ TOP_RIGHT_BORDER };
            display[LocalCoordToIndex({ 0, SHORT(size.Y - 1) })] = wchar_t{ BOTTOM_LEFT_BORDER };
            display[size.X * size.Y - 1] = wchar_t{ BOTTOM_RIGHT_BORDER };

            // Fill top
            for (int i = 1; i < size.X - 1; i++)
                display[i] = wchar_t{ HORIZONTAL_BORDER };
            // Fill bottom
            for (int i = LocalCoordToIndex({ 1, SHORT(size.Y - 1) }); i < LocalCoordToIndex({ SHORT(size.X - 1), SHORT(size.Y - 1) }); i++)
                display[i] = wchar_t{ HORIZONTAL_BORDER };
            // Fill left
            for (int i = 1; i < size.Y - 1; i++)
                display[LocalCoordToIndex({ 0, SHORT(i) })] = wchar_t{ VERTICAL_BORDER };
            // Fill right
            for (int i = 1; i < size.Y - 1; i++)
                display[LocalCoordToIndex({ SHORT(size.X - 1), SHORT(i) })] = wchar_t{ VERTICAL_BORDER };
        }
    };


    class Menu : public TextBase
    {
    public:
        const wchar_t* title = L" ";

        int selectionIndex = 0;
        //std::vector<std::shared_ptr<MenuOption>> options;
        int numOptions = 0;

        Menu() {
            display = new wchar_t[size.X * size.Y];
        };

        Menu(/*std::vector<std::shared_ptr<MenuOption>>& options, */const wchar_t* title)
        {
            //numOptions = options.size();
            // this->options = options;
            this->title = title;
            SHORT sizeX = 0;
            SHORT sizeY = numOptions + 3; // + 2 for borders, 1 for title
            /*
            for (size_t i = 0; i < numOptions; i++)
            {
                int len = wcslen(options[i]->name);
                if (len > sizeX)
                    sizeX = len;
            }
            */
            sizeX += 3; // 2 for borders, 1 for selection asterisk
            display = new wchar_t[sizeX * sizeY];
            this->size = { sizeX, sizeY };

            for (size_t i = 0; i < size.X * size.Y - 1; i++)
            {
                display[i] = *L" ";
            }

        }

        Menu(COORD size, int numOptions, const wchar_t* title)
        {
            selectionIndex = 0;
            this->title = title;
            display = new wchar_t[size.X * size.Y];
            this->size = size;
            this->numOptions = numOptions;
            // Initial fill
            for (size_t i = 0; i < size.X * size.Y - 1; i++)
            {
                display[i] = *L" ";
            }
            // Initialize Selection

        }

        void UpdateDisplay(COORD screenMousePosition)
        {
            COORD menuMouseCoord = ScreenSpaceCoordToMenuCoord(screenMousePosition);
            bool inBounds = menuMouseCoord.X >= 0 && menuMouseCoord.X < size.X - 1 && menuMouseCoord.Y >= 0 && menuMouseCoord.Y < size.Y - 1;
            int selection = menuMouseCoord.Y - 2;
            if (inBounds && !(selection > numOptions) && selection >= 0)
                selectionIndex = selection;

            // Fill corners
            display[0] = wchar_t{ TOP_LEFT_BORDER };
            display[LocalCoordToIndex({ SHORT(size.X - 1), 0 })] = wchar_t{ TOP_RIGHT_BORDER };
            display[LocalCoordToIndex({ 0, SHORT(size.Y - 1) })] = wchar_t{ BOTTOM_LEFT_BORDER };
            display[size.X * size.Y - 1] = wchar_t{ BOTTOM_RIGHT_BORDER };

            // Fill top
            for (int i = 1; i < size.X - 1; i++)
                display[i] = wchar_t{ HORIZONTAL_BORDER };
            // Fill bottom
            for (int i = LocalCoordToIndex({ 1, SHORT(size.Y - 1) }); i < LocalCoordToIndex({ SHORT(size.X - 1), SHORT(size.Y - 1) }); i++)
                display[i] = wchar_t{ HORIZONTAL_BORDER };
            // Fill left
            for (int i = 1; i < size.Y - 1; i++)
                display[LocalCoordToIndex({ 0, SHORT(i) })] = wchar_t{ VERTICAL_BORDER };
            // Fill right
            for (int i = 1; i < size.Y - 1; i++)
                display[LocalCoordToIndex({ SHORT(size.X - 1), SHORT(i) })] = wchar_t{ VERTICAL_BORDER };

            for (int i = 0; i < wcslen(title); i++)
            {
                display[LocalCoordToIndex({ SHORT(1 + i), SHORT(1) })] = title[i];
            }
            /*
            for (int i = 0; i < numOptions; i++)
            {
                if (i == selectionIndex)
                {
                    display[LocalCoordToIndex({ SHORT(1), SHORT(i + 2) })] = *L"*";
                }
                else
                    display[LocalCoordToIndex({ SHORT(1), SHORT(i + 2) })] = *L" ";
                for (size_t c = 0; c < wcslen(options[i]->name) && c < size.X - 3; c++)
                {
                    SHORT row = c + 2;
                    SHORT col = i + 2;
                    display[LocalCoordToIndex({ row, col })] = options[i]->name[c];
                }
            }
            */
        }

    private:
    };


}
