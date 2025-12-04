#include "framework.h"
#include <Core/Public/Request.h>

void Main()
{
    AllocConsole();
    FILE* F;
    freopen_s(&F, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Platinum V2 | https://github.com/1lunarxx/Platinum-V2");
    MH_Initialize();

    Request::Patch();

    static auto addr = Finder::FindPattern("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 48 8B F1 41 8B D8 48 8B 0D ? ? ? ?");
    std::cout << "realloc: " << addr << std::endl;

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