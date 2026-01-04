#include "pch.h"
#include "Core/Public/Pak.h"
#include "Core/Public/Finder.h"
#include <Globals.h>

void Pak::SigningKeyCheck(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6)
{
	static int Count = 0;
	Count++;

	if (Count >= 16)
	{
		MH_DisableHook((LPVOID)SigningKeyAddr);
	}

	if (a1)
	{
		*(int*)a1 = 0;
	}
}

void Pak::Patch()
{
	auto sRef = Finder::FindString(L"Invalid signing key", ImageBase);
	if (sRef && Finder::FN_Version() >= 14.00 && Finder::FN_Version() <= 17.30)
	{
		for (int i = 0; i < 2048; i++)
		{
			if (*(uint8_t*)(sRef - i) == 0x40 && *(uint8_t*)(sRef - i + 1) == 0x55)
			{
				SigningKeyAddr = sRef - i;
				Hook(sRef - i, SigningKeyCheck);
				break;
			}
		}
	}
}