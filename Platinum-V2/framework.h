#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <intrin.h>
#include "Minhook/MinHook.h"

inline uint64_t ImageBase = *(uint64_t*)(__readgsqword(0x60) + 0x10);

inline void Hook(uintptr_t offset, void* detour, void** og = nullptr)
{
	MH_CreateHook((LPVOID)offset, detour, (LPVOID*)og);
	MH_EnableHook((LPVOID)offset);
}