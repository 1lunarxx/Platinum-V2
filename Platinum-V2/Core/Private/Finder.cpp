#include "Core/Public/Finder.h"

// thanks ploosh
IMAGE_SECTION_HEADER* Finder::PE::GetSection(const char* name, uintptr_t module)
{
    IMAGE_DOS_HEADER* dos_hdr = (IMAGE_DOS_HEADER*)module;
    auto PEHeader = (IMAGE_NT_HEADERS*)(module + dos_hdr->e_lfanew);
    auto StartOfSects = IMAGE_FIRST_SECTION(PEHeader);

    for (int i = 0; i < PEHeader->FileHeader.NumberOfSections; i++)
    {
        IMAGE_SECTION_HEADER* Section = StartOfSects + i;

        if (strncmp((const char*)Section->Name, name, 8) == 0) return Section;
    }

    return nullptr;
}

uintptr_t Finder::FindString(const wchar_t* string, uintptr_t module)
{
    auto text = PE::GetSection(".text", module);
    auto rdata = PE::GetSection(".rdata", module);
    if (!text || !rdata || !string) return 0;

    uint8_t* textBase = reinterpret_cast<uint8_t*>(module + text->VirtualAddress);
    size_t textSize = text->Misc.VirtualSize;

    uint8_t* rdataBase = reinterpret_cast<uint8_t*>(module + rdata->VirtualAddress);
    uint8_t* rdataEnd = rdataBase + rdata->Misc.VirtualSize;

    for (size_t i = 0; i < textSize - 6; i++)
    {
        if (*(uint8_t*)(textBase + i) != 0x8D)  continue;

        int32_t rel = *(int32_t*)(textBase + i + 2);
        uint8_t* target = (textBase + i + 6) + rel;

        if (target < rdataBase || target >= rdataEnd) continue;
        if (wcscmp((wchar_t*)target, string) == 0) return reinterpret_cast<uintptr_t>(textBase + i - 1);
    }

    return 0;
}

std::vector<short> parsePattern(const char* pattern)
{
    std::vector<short> out{};
    auto patternLen = strlen(pattern);

    for (auto i = 0; i < patternLen; i++)
    {
        if (*(pattern + i) == '?') out.push_back(-1);
        else out.push_back((short)strtoul(pattern + i, nullptr, 16));

        i++;
        while (*(pattern + i) != ' ') i++;
    }

    return out;
}

uintptr_t Finder::FindPattern(const char* signature)
{
    auto text = PE::GetSection(".text", ImageBase);
    if (!text) return 0;

    auto patternBytes = parsePattern(signature);
    const size_t patternSize = patternBytes.size();
    const short* patternData = patternBytes.data();

    uint8_t* textBase = reinterpret_cast<uint8_t*>(ImageBase + text->VirtualAddress);
    size_t textSize = static_cast<size_t>(text->Misc.VirtualSize);

    for (size_t i = 0; i <= textSize - patternSize; ++i)
    {
        bool found = true;

        for (size_t j = 0; j < patternSize; ++j)
        {
            if (patternData[j] != -1 &&
                textBase[i + j] != static_cast<uint8_t>(patternData[j]))
            {
                found = false;
                break;
            }
        }

        if (found) return reinterpret_cast<uintptr_t>(&textBase[i]);
    }

    return 0;
}