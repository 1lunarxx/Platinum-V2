#include "Core/Public/Request.h"
#include <iostream>
#include <Globals.h>

bool Request::ProcessRequest(Containers::FCurlHttpRequest* HttpRequest)
{
    auto URL = HttpRequest->GetURL().ToString();
    const wchar_t* domains[] = { L"epicgames.com", L"epicgames.dev", L"epicgames.net" };

    static int Requests = 0;
    Requests++;

    if (Requests == 10)
    {
        auto EOSHandle = reinterpret_cast<uintptr_t>(LoadLibraryA("EOSSDK-Win64-Shipping.dll"));
        if (EOSHandle)
        {
            auto sRef = Finder::FindString(L"ProcessRequest failed. URL '%s' is not using a whitelisted domain. %p", EOSHandle);

            for (int i = 0; i < 2048; i++)
            {
                if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C)
                {
                    Hook(sRef - i, EOSProcessRequest, (void**)&Originals::EOSProcessRequest);
                    break;
                }
            }
        }
    }

    if (URL)
    {
        const wchar_t* endpoint = nullptr;

        for (const auto& domain : domains)
        {
            const wchar_t* found = std::wcsstr(URL, domain);
            if (found)
            {
                endpoint = found + wcslen(domain);
                break;
            }
        }

        if (endpoint)
        {
            std::wstring newUrl = BackendURL;

            if (*endpoint != L'/') newUrl += L'/';
            newUrl += endpoint;

            HttpRequest->SetURL(newUrl.c_str(), false);
        }
    }

	return Originals::ProcessRequest(HttpRequest);
}

bool Request::EOSProcessRequest(Containers::FCurlHttpRequest* HttpRequest)
{
    auto URL = HttpRequest->GetURL().ToString();
    const wchar_t* domains[] = { L"epicgames.com", L"epicgames.dev", L"epicgames.net", L"api.epicgames.dev"};

    if (URL)
    {
        const wchar_t* endpoint = nullptr;

        for (const auto& domain : domains)
        {
            const wchar_t* found = std::wcsstr(URL, domain);
            if (found)
            {
                endpoint = found + wcslen(domain);
                break;
            }
        }

        if (endpoint)
        {
            std::wstring newUrl = BackendURL;

            if (*endpoint != L'/') newUrl += L'/';
            newUrl += endpoint;

            HttpRequest->SetURL(newUrl.c_str(), true);
        }
    }

    return Originals::EOSProcessRequest(HttpRequest);
}

void Request::Patch()
{
    auto sRef = Finder::FindString(L"STAT_FCurlHttpRequest_ProcessRequest", ImageBase);
    if (!sRef) 
        sRef = Finder::FindString(L"%p: request (easy handle:%p) has been added to threaded queue for processing", ImageBase);

    if (!sRef)
    {
        // really gay fix
        Sleep(2500);
        sRef = Finder::FindString(L"%p: request (easy handle:%p) has been added to threaded queue for processing", ImageBase);
    }

    for (int i = 0; i < 2048; i++)
    {
        if (*(uint8_t*)(sRef - i) == 0x4C && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xDC)
        {
            Originals::ProcessRequest = reinterpret_cast<bool(*)(Containers::FCurlHttpRequest*)>(sRef - i);
            Hook(sRef - i, ProcessRequest, (void**)&Originals::ProcessRequest);
            break;
        }
        else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xC4)
        {
            Originals::ProcessRequest = reinterpret_cast<bool(*)(Containers::FCurlHttpRequest*)>(sRef - i);
            Hook(sRef - i, ProcessRequest, (void**)&Originals::ProcessRequest);
            break;
        }
        else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x81 && *(uint8_t*)(sRef - i + 2) == 0xEC || *(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x83 && *(uint8_t*)(sRef - i + 2) == 0xEC)
        {
            for (int x = 0; x < 50; x++)
            {
                if (*(uint8_t*)(sRef - i - x) == 0x40)
                {
                    // later
                }
            }
        }
    } 
}