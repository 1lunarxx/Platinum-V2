#include "pch.h"
#include "Core/Public/Misc.h"
#include <Globals.h>

void Misc::SelectEdit(void* a1)
{
	auto sRef = Finder::FindString(L"CompleteBuildingEditInteraction", ImageBase);
	uintptr_t CompleteBuildingEditInteractionLea = 0;

	for (int i = 1; i < 2000; i++)
	{
		if (*(uint8_t*)(sRef - i) == 0x4C && *(uint8_t*)(sRef - i + 1) == 0x8D)
		{
			CompleteBuildingEditInteractionLea = sRef - i;
			break;
		}
		else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8D)
		{
			CompleteBuildingEditInteractionLea = sRef - i;
			break;
		}
	}

	Originals::CompleteBuildingEditInteraction = (char (*)(void*))Finder::RelativeOffset(CompleteBuildingEditInteractionLea, 3);

	if (bEOR)
	{
		Originals::CompleteBuildingEditInteraction(a1);
	}

	return Originals::SelectEdit(a1);
}

void Misc::Thread()
{
	while (true)
	{
		if (GetKeyState(VK_F2))
		{
			bEOR = true;
		}
	}
}

void Misc::Patch()
{
	if (Finder::FN_Version() <= 10.40)
	{
		auto sRef = Finder::FindString(L"EditModeInputComponent0", ImageBase);
		uintptr_t SelectEditOffset = 0;

		for (int i = 0; i < 2048; i++)
		{
			if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8D && *(uint8_t*)(sRef - i + 2) == 0x05)
			{
				SelectEditOffset = sRef - i;
				break;
			}
		}

		if (SelectEditOffset != 0)
		{
			Hook(Finder::RelativeOffset(SelectEditOffset, 3), SelectEdit, (void**)&Originals::SelectEdit);
		}
	}
}