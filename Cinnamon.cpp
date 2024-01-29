#include "engine.h"
#include "Player.h";
#include "Empty.hpp"
using namespace Cinnamon;

//------------------------------------------ TODO ----------------------------------------------
// 
// 
// DONE:
//      - World Space System 
//      - collision system
//      - Mouseclick interaction
//      - Interactable WorldObjects
//      - Fix mouse click bug
//      - Character WorldObject Proximity 
//      - Animation system 
//      - Event System
// 
// IN PROGRESS:
//      - Inventory System
//      - UI
//      - UI RENDER
// 
// PLANNED:
//      - Non-worldobject Items
//      - Multithreading & timers
//      - Map Tool
// 
//-----------------------------------------------------------------------------------------
// 
// 
// 
// ------------------- WORLDOBJECT CHARACTERS --------------------------------------
/*
// WALLS
#define VERTICAL_WALL       0x2503
#define HORIZONTAL_WALL     0x2501
#define TOP_LEFT_WALL       0x250F
#define TOP_RIGHT_WALL      0x2513
#define BOTTOM_LEFT_WALL    0x2517
#define BOTTOM_RIGHT_WALL   0x251B


// INTERACTABLES
#define CHEST               0x25A1
#define DOOR_CLOSED         0x2500
#define DOOR_OPEN           L' '
#define FLOWER_1_FRAME_BANK L'*', L'^'

// MENU
#define VERTICAL_BORDER     0x2551
#define HORIZONTAL_BORDER   0x2550
#define TOP_LEFT_BORDER     0x2554
#define TOP_RIGHT_BORDER    0x2557
#define BOTTOM_LEFT_BORDER  0x255A
#define BOTTOM_RIGHT_BORDER 0x255D

std::chrono::duration<double> animationInterval = std::chrono::milliseconds(500);
std::chrono::duration<double> animationTime = std::chrono::seconds(0);

// Debug


void DebugLog(const wchar_t* msg)
{
    wcscpy_s(debugMsg, msg);
}

void DebugLog(int msg)
{
    DebugLog(std::to_wstring(msg).c_str());
}

void DebugLog(COORD coord)
{
    std::wstring msg = std::to_wstring(coord.X);
    msg.append(L", ");
    msg.append(std::to_wstring(coord.Y));
    DebugLog(msg.c_str());
}

// Base for all game entities
class GameObject_
{
public:
    const wchar_t* name = L"NULL OBJECT NAME";
    wchar_t* dispChar = new wchar_t{ L' ' };
    const wchar_t* description = L"NULL DESCRIPTION";

    GameObject_()
    {
        dispChar = new wchar_t{ L' ' };
    }

    virtual ~GameObject_() = default;
};


class WorldObject : public GameObject_
{
public:
    bool bCollision = false;

    WorldObject()
    {
        dispChar = new wchar_t{ L' ' };
    };

    WorldObject(wchar_t wChar, bool bCollision)
    {
        this->bCollision = bCollision;
        *dispChar = wChar;
    }

    WorldObject(wchar_t* wChar, bool bCollision)
    {
        this->bCollision = bCollision;
        dispChar = wChar;
    }

    void SetWorldPosition(COORD newPos)
    {
        WorldPosition = newPos;
    }

    COORD GetWorldPosition()
    {
        return this->WorldPosition;
    }

    virtual ~WorldObject() = default;

protected:
    COORD WorldPosition = COORD();
};


// --------------------------- UI ------------------------------------

class MenuOption
{
public:
    const wchar_t* name = L" ";
    std::function<void(void)> method;
    
    MenuOption() {}

    MenuOption(const wchar_t* name, std::function<void(void)> method)
    {
        this->name = name;
        this->method = method;
    }

    void OnConfirm()
    {
        method();
    }
};

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
    std::vector<MenuOption*> options;
    int numOptions = 0;
    
    Menu() {
        display = new wchar_t[size.X * size.Y];
    };
    
    Menu(std::vector<MenuOption*> &options, const wchar_t* title)
    {
        numOptions = options.size();
        this->options = options;
        this->title = title;
        SHORT sizeX = 0;
        SHORT sizeY = numOptions + 3; // + 2 for borders, 1 for title

        for (size_t i = 0; i < numOptions; i++)
        {
            int len = wcslen(options[i]->name);
            if (len > sizeX)
                sizeX = len;
        }
        
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
        for (int i = LocalCoordToIndex({ 1, SHORT(size.Y - 1) }); i < LocalCoordToIndex({ SHORT(size.X - 1), SHORT(size.Y - 1)}); i++)
            display[i] = wchar_t{ HORIZONTAL_BORDER };
        // Fill left
        for (int i = 1; i < size.Y - 1; i++)
            display[LocalCoordToIndex({ 0, SHORT(i) })] = wchar_t{ VERTICAL_BORDER };
        // Fill right
        for (int i = 1; i < size.Y - 1; i++)
            display[LocalCoordToIndex({ SHORT(size.X - 1), SHORT(i) })] = wchar_t{VERTICAL_BORDER};
        
        for (int i = 0; i < wcslen(title); i++)
        {
            display[LocalCoordToIndex({ SHORT(1 + i), SHORT(1)})] = title[i];
        }

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
    }

private:
};


class TextBox :  public TextBase
{
public:
    TextBox()
    {
        size = { csbi.dwSize.X, SHORT(csbi.dwSize.Y / 3) };
        this->display = new wchar_t[size.X * size.Y];
        screenPosition = { 0, SHORT(size.Y * 2) };
    }
};
// ----------------------- INTERFACES --------------------------------

class IHaveMenu
{
public:
    Menu menu;

    IHaveMenu() {};

    virtual ~IHaveMenu() = default;

};


class IHaveRightClickMenu
{
public:
    Menu menu;
    TextBox inspectionTextBox;


    IHaveRightClickMenu()
    {
        std::vector<MenuOption*> ops;
        std::function<void(void)> inspect;
        menu.options.push_back(new MenuOption(
            L"Inspect",
            *new std::function<void(void)>(inspect)
        ));
    }
    
    void Inspect()
    {
        
    }


    virtual ~IHaveRightClickMenu() = default;
};


class IAmAnimated : public WorldObject
{
public:
    bool bAnimActive = false;
    wchar_t* frameBank;
    int frameIndex = 0;
    int frameBankSize = 0;

    virtual void AnimationUpdate()
    {
        int len = frameBankSize / sizeof(wchar_t);
        if (frameIndex == len - 3)
        {
            *dispChar = frameBank[0];
            frameIndex = 0;
        }
        else
        {
            *dispChar = frameBank[frameIndex + 1];

            frameIndex++;
        }
    };

    virtual ~IAmAnimated() = default;
};


class IAmInteractable
{
public:
    virtual void OnInteract() { OutputDebugStringW(L"BASE ONINTERACT CALLED\n"); };

    virtual ~IAmInteractable() = default;
};


class IHaveInventory
{
public:
    GameObject_* Inventory;

    IHaveInventory()
    {
        
    }
    virtual ~IHaveInventory() {};
};


class Door : public WorldObject, public IAmInteractable
{
public:
    bool bOpen = true;
    wchar_t openChar = wchar_t{ DOOR_OPEN };
    wchar_t closedChar = wchar_t{ DOOR_CLOSED };


    Door(bool bOpen)
    {
        this->bOpen = bOpen;
        bCollision = !bOpen;
        dispChar = bOpen ? &openChar : &closedChar;
    };

    void OnInteract() override
    {
        bOpen = !bOpen;
        bCollision = !bCollision;
        dispChar = bOpen ? &openChar : &closedChar;
    };
};


class Flower : public IAmInteractable, public IAmAnimated, public IHaveRightClickMenu
{
public:
    Flower()
    {
        frameBank = new wchar_t [2] { FLOWER_1_FRAME_BANK };
        frameBankSize = sizeof frameBank;
        name = L"Flower";
        bCollision = false;
        *dispChar = frameBank[0];

    }
};


class Chest : public IAmInteractable, public IHaveInventory, public WorldObject
{
public:
    Chest()
    {
        dispChar = new wchar_t { CHEST };
    }

    void OnInteract() override
    {
    }
};


class WorldMap
{
public:
    WorldObject** map;
    DWORD size = DWORD();
    COORD dimSize = COORD();
    COORD spawn;

    WorldMap()
    {
        map = new WorldObject*[cellCount];
        size = cellCount;
        dimSize.X = csbi.dwSize.X;
        dimSize.Y = csbi.dwSize.Y;
    };

    WorldMap(int width, int height)
    {
        size = width * height;
        dimSize.X = width;
        dimSize.Y = height;
        map = new WorldObject*[size];
    };

    int CoordToIndex(COORD coord)
    {
        int x = int(coord.X);
        int y = int(coord.Y);

        return x + y + (y * dimSize.X);
    };

    bool OutOfBounds(COORD pos)
    {
        return pos.X < 0 || pos.Y < 0 || pos.X > dimSize.X || pos.Y > dimSize.Y;
    }
};


WorldMap GetTestMap()
{
    WorldMap* testMap = new WorldMap(200, 200);
    for (size_t i = 0; i < testMap->size; i++)
    {
        testMap->map[i] = new WorldObject(L'.', false);
    }

    testMap->spawn = COORD { 23, 12 };

    for (SHORT x = 25; x < 26; x++)
    {
        for (SHORT y = 0; y < 10; y++)
        {
            testMap->map[testMap->CoordToIndex(COORD {x, y})] = 
                new WorldObject(wchar_t{ VERTICAL_WALL }, true);
        }
    }

    for (SHORT x = 55; x < 56; x++)
    {
        for (SHORT y = 0; y < 10; y++)
        {
            testMap->map[testMap->CoordToIndex(COORD{ x, y })] =
                new WorldObject(wchar_t{ VERTICAL_WALL }, true);
        }
    }

    testMap->map[testMap->CoordToIndex(COORD{ 25, 9 })] =
        new WorldObject(wchar_t{ BOTTOM_LEFT_WALL }, true);

    for (SHORT x = 26; x < 55; x++)
    {
        for (SHORT y = 9; y < 10; y++)
        {
            testMap->map[testMap->CoordToIndex(COORD {x, y})] =
                new WorldObject(wchar_t{ HORIZONTAL_WALL}, true);
        }
    }

    testMap->map[testMap->CoordToIndex(COORD{ (25 + (55 - 25) / 3), 9 })] =
        new Door(false);
    testMap->map[testMap->CoordToIndex(COORD{ 55, 9 })] =
        new WorldObject(wchar_t{ BOTTOM_RIGHT_WALL }, true);
    testMap->map[testMap->CoordToIndex(COORD{ 40, 8 })] =
        new WorldObject(wchar_t{ CHEST }, true);
    testMap->map[testMap->CoordToIndex(COORD{ 40, 14 })] =
        new Flower();

    return *testMap;
};

// Character ------------------------------------------------------------------------------

class Character : public WorldObject, public IHaveInventory
{
public:
    double velocity = 5;


    Character() {
        SetWorldPosition(COORD{ 0, 0 });
        dispChar = new wchar_t{ L'O' };
    };

    double* GetPosition()
    {
        return Position;
    };

    void SetPosition(double newPos[2])
    {
        for (size_t i = 0; i < 2; i++)
        {
            Position[i] = newPos[i];
        }
        UpdateWorldPosition();
    };


private:
    double Position[2] = { 0, 0 };

    void UpdateWorldPosition()
    {
        COORD newPos = { 0, 0 };
        newPos.X = static_cast<SHORT>(floor(Position[0]));
        newPos.Y = static_cast<SHORT>(floor(Position[1]));

        SetWorldPosition(newPos);
    };
};


// Handles world space conversion, camera view type, writing to screen_to_display
class Viewport
{
public:
    WorldMap* worldMap;
    Character* character;
    wchar_t* view = new wchar_t[cellCount];
    COORD* viewSize = &csbi.dwSize;
    // Start of screen view in world coords
    COORD viewRef = { 0, 0 };
    const wchar_t* msg = L" ";
    std::vector<Menu*> activeMenus;
    


    Viewport()
    {
        worldMap = new WorldMap();
        character = new Character();
    };

    Viewport(WorldMap& map, Character& c)
    {
        worldMap = &map;
        character = &c;
        UpdateView();
    };
    
    COORD GetScreenMousePosition()
    {
        return { irec.Event.MouseEvent.dwMousePosition.X, irec.Event.MouseEvent.dwMousePosition.Y };
    }
    
    COORD GetWorldMousePosition()
    {
        return SSCoordToWS({ irec.Event.MouseEvent.dwMousePosition.X, irec.Event.MouseEvent.dwMousePosition.Y });
    }

    // Writes world space to view
    void UpdateView()
    {
        // Convert from world space coord positions to linear view space
        int worldMapStartX = character->GetWorldPosition().X - (int)floor(viewSize->X / 2);
        int worldMapStartY = character->GetWorldPosition().Y - (int)floor(viewSize->Y / 2);
        int worldMapEndX = character->GetWorldPosition().X + (int)floor(viewSize->X / 2);
        int worldMapEndY = character->GetWorldPosition().Y + (int)floor(viewSize->Y / 2);

        for (int x = worldMapStartX; x < worldMapEndX; x++)
        {
            for (int y = worldMapStartY; y < worldMapEndY; y++)
            {
                if (x == character->GetWorldPosition().X && y == character->GetWorldPosition().Y)
                    view[x - worldMapStartX + ((y - worldMapStartY) * int(viewSize->X))] = *character->dispChar;

                else if (x < 0 || y < 0 || x > worldMap->dimSize.X || y > worldMap->dimSize.Y)
                    view[x - worldMapStartX + ((y - worldMapStartY) * int(viewSize->X))] = L' ';

                else
                    view[x - worldMapStartX + ((y - worldMapStartY) * int(viewSize->X))] = 
                    *worldMap->map[worldMap->CoordToIndex(COORD{SHORT(x), SHORT(y)})]->dispChar;
            }
        }

        for (size_t i = 0; i < wcslen(debugMsg); i++)
        {
            view[i] = debugMsg[i];
        }

        // Store reference point for SSCoordToWS conversion
        viewRef = { (SHORT)worldMapStartX, (SHORT)worldMapStartY };
    };
    

    void UpdateUI()
    {
        for (int i = 0; i < activeMenus.size(); i++)
        {
            activeMenus[i]->UpdateDisplay(GetScreenMousePosition());
            int xOffset = activeMenus[i]->screenPosition.X;
            int yOffset = activeMenus[i]->screenPosition.Y;
            
            for (int x = 0; x < activeMenus[i]->size.X; x++)
            {
                for (int y = 0; y < activeMenus[i]->size.Y; y++)
                {
                    int viewIndex = ScreenCoordToIndex({ SHORT(xOffset + x), SHORT(yOffset + y) });

                    view[viewIndex] = activeMenus[i]->display[activeMenus[i]->LocalCoordToIndex({SHORT(x), SHORT(y)})];
                }
            }
        }
    }

    void DrawView()
    {
        size_t textlen = wcslen(view);
        DWORD* txtlenout = new DWORD{ 0 };
        WriteConsoleOutputCharacterW(hStdOut, view, (DWORD)textlen, homeCoords, txtlenout);
        delete txtlenout;
    };

    COORD SSCoordToWS(COORD Pos)
    {
        COORD WSCoord{ viewRef.X + Pos.X, viewRef.Y + Pos.Y };
        return WSCoord;
    }
    
    int ScreenCoordToIndex(COORD pos)
    {
        return pos.X + (pos.Y * viewSize->X);
    }

    bool PositionInCharacterProximity(COORD pos, int range = 1)
    {
        int charPos [2] = {(int)character->GetWorldPosition().X, (int)character->GetWorldPosition().Y};
        int startX = charPos[0] - range;
        int startY = charPos[1] - range;
        int endX = charPos[0] + range + 1;
        int endY = charPos[1] + range + 1;

        bool outOfBounds = worldMap->OutOfBounds(pos);
        bool xCheck = ((int)pos.X == charPos[0]);
        bool yCheck = ((int)pos.Y == charPos[1]);
        bool posNotCharacterPos =  !(xCheck && yCheck);

        for (int x = startX; x < endX; x++)
        {
            for (int y = startY; y < endY; y++)
            {
                if (pos.X == x && 
                    pos.Y == y && 
                    !outOfBounds &&
                    posNotCharacterPos)
                    return true;
            }
        }

        return false;
    }

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

    void AnimationStep()
    {
        for (size_t i = 0; i < worldMap->size; i++)
        {
            IAmAnimated* obj = dynamic_cast<IAmAnimated*>(worldMap->map[i]);
            if (obj != nullptr)
                obj->AnimationUpdate();
        }
    }
};
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// DEFINE GAME OBJECTS HERE
//-----------------------------------------------------------------------------------------

WorldMap worldMap = GetTestMap();
Character character;

//-----------------------------------------------------------------------------------------
// GAME LOGIC
//-----------------------------------------------------------------------------------------
void GameLoop(Viewport viewport)
{
    PeekConsoleInputW(hStdIn, &irec, 1, &events);
    start_frame = std::chrono::steady_clock::now();

    if (bFrame)
    {
        deltaTime = FirstFrameTime.count() - SecondFrameTime.count();
    }
    else
    {
        deltaTime = SecondFrameTime.count() - FirstFrameTime.count();
    }

    viewport.UpdateView();
    viewport.UpdateUI();
	viewport.HandleInput(deltaTime);
    viewport.DrawView();

    if (animationTime > animationInterval)
    {
        viewport.AnimationStep();
        animationTime = std::chrono::duration<double>(0);
    }

    std::chrono::duration<double> elapsed = (std::chrono::steady_clock::now() - start_frame);

    if (bFrame)
        FirstFrameTime = elapsed;
    else
        SecondFrameTime = elapsed;

    animationTime = animationTime + elapsed;
	FlushConsoleInputBuffer(hStdIn);
};


void timer_start(std::function<void()> func, unsigned int interval)
{
    
};


int main2()
{

    // Initialize console stuff ---------------------------------------------------------------------------

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return 0;

    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return 0;
    SetConsoleScreenBufferSize(hStdOut, COORD{ 120, 30 });
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    GetConsoleCursorInfo(hStdOut, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(hStdOut, &cursorInfo);

    SetConsoleMode(hStdIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    SetConsoleMode(hStdIn, ~ENABLE_QUICK_EDIT_MODE);



    //-----------------------------------------------------------------------------------------------------

    character.SetPosition(new double[2] { double(worldMap.spawn.X), double(worldMap.spawn.Y) });

    Viewport viewport = Viewport(worldMap, character);
    std::vector<MenuOption*> ops{
        new MenuOption(L"Resume", nullptr),
        new MenuOption(L"Options", nullptr),
        new MenuOption(L"Exit", nullptr)
    };

    Menu testMenu = Menu(ops, L"PAUSED");
    testMenu.screenPosition = { 30, 15 };
    viewport.activeMenus.push_back(&testMenu);
    
    while (true)
    {
        GameLoop(viewport);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

};
*/




int main() {
    
    auto level = new Level();
    level->size = { 120, 30 };
    for (int x = 0; x < level->size.X - 1; x++)
    {
        for (int y = 0; y < level->size.Y - 1; y++)
        {
            auto e = new Empty();
            e->position = { double(x), double(y) };
            level->gameObjects.emplace_back(e);
        }
    }

	auto player = new Player();
    player->position = { 30, 5 };
    player->layer = 0;
    level->gameObjects.emplace_back(player);

	    Game::Instance().InitializeGame(level);
    DebugLog("Level size: ", Game::Instance().activeLevel.gameObjects.size());
    for (size_t i = 0; i < Game::Instance().activeLevel.gameObjects.size(); i++) {
        DebugLog("Position: ", Game::Instance().activeLevel.gameObjects[i]->position);
    }
    
    Game::Instance().StartLevel();
    }