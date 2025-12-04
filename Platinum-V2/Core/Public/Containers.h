#pragma once
#include "framework.h"
#include "Core/Public/Finder.h"
#include <iostream>

namespace Containers
{
	class FMemory {
	public:
		static inline void* InternalRealloc(void* _a1, __int64 _a2, unsigned int _a3) {
			static auto addr = Finder::FindPattern("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 48 8B F1 41 8B D8 48 8B 0D ? ? ? ?");
			std::cout << "realloc: " << addr << std::endl;

			auto ret = reinterpret_cast<void* (*)(void*, __int64, unsigned int)>(addr);
			return ret(_a1, _a2, _a3);
		}

		template<typename T = void>
		static inline T* Realloc(void* Ptr, uint64_t Size, uint32_t Alignment = 0x0) {
			return static_cast<T*>(InternalRealloc(Ptr, Size, Alignment));
		}

		template<typename T = void>
		static inline T* Malloc(uint64_t Size, uint32_t Alignment = 0x0) {
			return Realloc<T>(nullptr, Size, Alignment);
		}

		static inline void Free(void* ptr) {
			Realloc(ptr, 0, 0);
		}
	};

	template<typename ArrayElementType>
	class TArray {
	protected:
		ArrayElementType* Data;
		int32_t NumElements;
		int32_t MaxElements;
	public:
		void Free() {
			FMemory::Free(Data);
			NumElements = 0;
			MaxElements = 0;
		}
	};

	class FString : public TArray<wchar_t> {
	public:
		FString() : TArray<wchar_t>() {}

		FString(const wchar_t* str);
		wchar_t* ToString();
	};

	class FCurlHttpRequest
	{
	public:
		void** VTable;

		FString GetURL();
		void SetURL(const wchar_t* url);
	};
}