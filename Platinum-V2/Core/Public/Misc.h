#pragma once
#include "framework.h"
#include "Core/Public/Finder.h"

namespace Misc
{
	namespace Originals
	{
		static char (*CompleteBuildingEditInteraction)(void* a1);
		static void (*SelectEdit)(void* a1);
	}

	void SelectEdit(void* a1);
	
	void Thread();
	void Patch();
}