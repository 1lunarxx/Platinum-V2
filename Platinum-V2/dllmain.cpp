#include "pch.h"
#include <Core/Public/Request.h>
#include <Globals.h>
#include <Core/Public/Hooks.h>
#include <Core/Public/Pak.h>
#include <Core/Public/Misc.h>

void Main()
{
    if (bConsole)
    {
        AllocConsole();
        FILE* F;
        freopen_s(&F, "CONOUT$", "w", stdout);
        SetConsoleTitleA("Platinum V2 | https://github.com/1lunarxx/Platinum-V2");
    }

    MH_Initialize();

    Request::Patch();
    Hooks::Patch();
    Pak::Patch();

    if (bMiscOptions)
    {
        Misc::Patch();
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Misc::Thread, 0, 0, 0);
    }
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
    switch (Reason)
    {
    case 1: 
        std::thread(Main).detach();
    case 0:
        break;
    }
    return 1;
}