#include "framework.h"
#include <Core/Public/Request.h>
#include <Core/Public/Hooks.h>
#include <Globals.h>

void Main()
{
    AllocConsole();
    FILE* F;
    freopen_s(&F, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Platinum V2 | https://github.com/1lunarxx/Platinum-V2");
    MH_Initialize();

    Request::Patch();
    if (FN_Version >= 29) Hooks::Patch();

    MH_EnableHook(MH_ALL_HOOKS);
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