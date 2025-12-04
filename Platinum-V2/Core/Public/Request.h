#pragma once
#include "framework.h"
#include "Core/Public/Containers.h"

namespace Request
{
	namespace Originals
	{
		static void (*ProcessRequest)(Containers::FCurlHttpRequest*);
		static void (*EOSProcessRequest)(Containers::FCurlHttpRequest*);
	}

	static void ProcessRequest(Containers::FCurlHttpRequest*);
	static void EOSProcessRequest(Containers::FCurlHttpRequest*);

	void Patch();
}