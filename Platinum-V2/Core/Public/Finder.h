#pragma once
#include "framework.h"

enum EModuleType
{
    Text,
    RData
};

namespace Finder
{
    static std::vector<short> parsePattern(const char* pattern)
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

	namespace PE
	{
		IMAGE_SECTION_HEADER* GetSection(const char* name, uintptr_t module);
	}

	uintptr_t FindString(const wchar_t* string, uintptr_t module);
	uintptr_t FindPattern(const char* signature, EModuleType Type = EModuleType::Text);
    uintptr_t RelativeOffset(uintptr_t ptr, int rel32Off);

    double FN_Version();
}