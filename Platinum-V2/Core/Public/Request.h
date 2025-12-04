#pragma once
#include "framework.h"
#include "Core/Public/Containers.h"

namespace Request
{
	namespace Originals
	{
		static bool (*ProcessRequest)(Containers::FCurlHttpRequest*);
		static bool (*EOSProcessRequest)(Containers::FCurlHttpRequest*);
	}

	static bool ProcessRequest(Containers::FCurlHttpRequest*);
	static bool EOSProcessRequest(Containers::FCurlHttpRequest*);

	void Patch();
}