#include "framework.h"
#include <Core/Public/Request.h>
#include <Globals.h>
#include <Core/Public/Hooks.h>
#include <Core/Public/Pak.h>

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

    //std::cout << "fnver: " << Finder::FN_Version() << std::endl;
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
    switch (Reason)
    {
    case 1: std::thread(Main).detach();
    case 0: break;
    }
    return 1;
}