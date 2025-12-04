#pragma once
#include "framework.h"

namespace Finder
{
	namespace PE
	{
		IMAGE_SECTION_HEADER* GetSection(const char* name, uintptr_t module);
	}

	uintptr_t FindString(const wchar_t* string, uintptr_t module);
	uintptr_t FindPattern(const char* signature);
}