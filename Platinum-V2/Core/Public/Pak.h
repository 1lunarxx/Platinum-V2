#pragma once
#include "framework.h"

namespace Pak
{
	static uintptr_t SigningKeyAddr = 0;

	void SigningKeyCheck(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6);
	void Patch();
}