#include "Core/Public/Hooks.h"
#include <Core/Public/Finder.h>
#include <Globals.h>

void Hooks::Patch()
{
    if (Finder::FN_Version() < 29.00)
        return;

	// UnsafeEnvorimentPopup
	{
        static const char* Patterns[] =
        {
            "4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89 73 F0 49 89 7B E8 48 8B F9 4D 89 63 E0 4D 8B E0 4D 89 6B D8",
            "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 41 0F B6 D8 48 89 55 ? 88 5C 24 ?",
            "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B DA 48 8B F1",
            "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? ? 0F B6 ? 44 88 44 24 ?",
            "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 0F B6 F8",
            "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? ? 0F B6 ?",
            "4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ?"
        };

        uintptr_t Addr = 0;

        for (auto& p : Patterns)
        {
            Addr = Finder::FindPattern(p);
            if (Addr) break;
        }

        if (!Addr) 
            return;

        DWORD oldProt;
        VirtualProtect((LPVOID)Addr, 1, PAGE_EXECUTE_READWRITE, &oldProt);
        *(uint8_t*)Addr = 0xC3;
        VirtualProtect((LPVOID)Addr, 1, oldProt, &oldProt);
	}

    // RequestExit
    {

    }
}