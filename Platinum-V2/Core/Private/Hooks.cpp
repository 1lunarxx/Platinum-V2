#include "pch.h"
#include "Core/Public/Hooks.h"
#include <Core/Public/Finder.h>
#include <Globals.h>

bool Hooks::SecurityCheck(__int64 a1)
{
    return false;
}

void Hooks::Patch()
{
    if (Finder::FN_Version() < 10.00)
        return;

    // Failed Security Check || Tested on 16.40
    if (Finder::FN_Version() >= 16.00 && Finder::FN_Version() <= 17.30)
    {
        uintptr_t sRef = Finder::FindString(L"bPassSecurityCheck", ImageBase);
        uint8_t* Addr = 0;

        if (sRef)
        {
            for (int i = 0; i < 0x400; i++)
            {
                uint8_t* p = (uint8_t*)(sRef - i);
                if (p[0] == 0x48 && p[1] == 0x89 && p[2] == 0x5C)
                {
                    Addr = p;
                    break;
                }
            }
        }

        if (Addr)
        {
            DWORD oldProt;
            VirtualProtect((LPVOID)Addr, 1, PAGE_EXECUTE_READWRITE, &oldProt);
            *(uint8_t*)Addr = 0xC3;
            VirtualProtect((LPVOID)Addr, 1, oldProt, &oldProt);
        }
    }

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
        static const char* Patterns[] =
        {
            "48 89 5C 24 ? 57 48 83 EC 40 41 B9 ? ? ? ? 0F B6 F9 44 38 0D ? ? ? ? 0F B6 DA 72 24 89 5C 24 30 48 8D 05 ? ? ? ? 89 7C 24 28 4C 8D 05 ? ? ? ? 33 D2 48 89 44 24 ? 33 C9 E8 ? ? ? ?",
            "48 8B C4 48 89 58 18 88 50 10 88 48 08 57 48 83 EC 30",
            "4C 8B DC 49 89 5B 08 49 89 6B 10 49 89 73 18 49 89 7B 20 41 56 48 83 EC 30 80 3D ? ? ? ? ? 49 8B",
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
}
