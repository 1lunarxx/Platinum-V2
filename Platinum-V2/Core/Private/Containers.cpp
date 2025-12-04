#include "Core/Public/Containers.h"
#include <Globals.h>

Containers::FString::FString(const wchar_t* str)
{
    if (!str || !*str) {
        Data = nullptr;
        NumElements = MaxElements = 0;
        return;
    }

    NumElements = MaxElements = static_cast<int>(wcslen(str)) + 1;
    Data = static_cast<wchar_t*>(FMemory::Malloc(NumElements * sizeof(wchar_t)));
    memcpy_s(Data, NumElements * sizeof(wchar_t), str, NumElements * sizeof(wchar_t));
}

wchar_t* Containers::FString::ToString()
{
    return Data;
}

Containers::FString Containers::FCurlHttpRequest::GetURL()
{
    FString Result;
    return ((FString & (*)(FCurlHttpRequest*, FString&))(*VTable))(this, Result);
}

void Containers::FCurlHttpRequest::SetURL(const wchar_t* url)
{
    // scuffed, cba to make a fortnite ver checker
    if (Season >= 29)
    {
        // I MANUALLY FOUND THIS BY TRYING ALL NUMBERS
        FString s(url);
        auto fn = (void(*)(FCurlHttpRequest*, FString&))VTable[13];
        fn(this, s);
    }
    else
    {
        FString s(url);
        auto fn = (void(*)(FCurlHttpRequest*, FString&))VTable[10];
        fn(this, s);
    }
}