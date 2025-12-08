#include "Core/Public/Finder.h"
#include <iostream>

IMAGE_SECTION_HEADER* Finder::PE::GetSection(const char* name, uintptr_t module)
{
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)module;
    IMAGE_NT_HEADERS* net_headers = (IMAGE_NT_HEADERS*)(module + dos_header->e_lfanew);
    auto StartOfSects = IMAGE_FIRST_SECTION(net_headers);

    for (int i = 0; i < net_headers->FileHeader.NumberOfSections; i++)
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
            if (patternData[j] != -1 && textBase[i + j] != static_cast<uint8_t>(patternData[j]))
            {
                found = false;
                break;
            }
        }

        if (found) 
            return reinterpret_cast<uintptr_t>(&textBase[i]);
    }

    return 0;
}

uintptr_t Finder::FindPatternR(const char* signature)
{
    auto rdata = Finder::PE::GetSection(".rdata", ImageBase);
    if (!rdata) 
        return 0;

    auto patternBytes = Finder::parsePattern(signature);
    const size_t patternSize = patternBytes.size();
    const short* patternData = patternBytes.data();

    uint8_t* base = reinterpret_cast<uint8_t*>(ImageBase + rdata->VirtualAddress);
    size_t size = static_cast<size_t>(rdata->Misc.VirtualSize);

    for (size_t i = 0; i <= size - patternSize; ++i)
    {
        bool found = true;

        for (size_t j = 0; j < patternSize; ++j)
        {
            if (patternData[j] != -1 &&
                base[i + j] != static_cast<uint8_t>(patternData[j]))
            {
                found = false;
                break;
            }
        }

        if (found)  
           return reinterpret_cast<uintptr_t>(&base[i]);
    }

    return 0;
}

double Finder::FN_Version()
{
    const wchar_t* cl_string = reinterpret_cast<const wchar_t*>(FindPatternR("2b 00 2b 00 46 00 6f 00 72 00 74 00 6e 00 69 00 74 00 65 00 2b 00 52 00 65 00 6c 00 65 00 61 00 73 00 65 00 2d 00"));
    if (!cl_string)
        return 67.67;

    const wchar_t* VStart = wcschr(cl_string, L'-');
    if (!VStart)
        return 67.67;
    VStart += 1; 

    const wchar_t* VEnd = wcschr(VStart, L'-');
    if (!VEnd) VEnd = cl_string + wcslen(cl_string); 

    size_t wcharCount = VEnd - VStart;

    wchar_t* temp = (wchar_t*)malloc((wcharCount + 1) * sizeof(wchar_t));
    memcpy(temp, VStart, wcharCount * sizeof(wchar_t));
    temp[wcharCount] = L'\0';

    wchar_t* endPtr;
    double version = wcstod(temp, &endPtr);

    free(temp);
    return version;
}