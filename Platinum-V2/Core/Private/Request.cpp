#include "Core/Public/Request.h"
#include <iostream>
#include <Globals.h>

void Request::ProcessRequest(Containers::FCurlHttpRequest* HttpRequest)
{
    auto URL = HttpRequest->GetURL().ToString();
    const wchar_t* domains[] = { L"epicgames.com", L"epicgames.dev", L"epicgames.net" };

    static int Requests = 0;
    Requests++;

    if (Requests == 10)
    {
        if (bEOS)
        {
            auto EOSHandle = reinterpret_cast<uintptr_t>(GetModuleHandleA("EOSSDK-Win64-Shipping.dll"));
            if (EOSHandle)
            {
                for (int i = 0; i < 2048; i++)
                {
                    auto sRef = Finder::FindString(L"ProcessRequest failed. URL '%s' is not using a whitelisted domain. %p", EOSHandle);
                    if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x89 && *(uint8_t*)(sRef - i + 2) == 0x5C)
                    {
                        Hook(sRef - i, EOSProcessRequest, (void**)&Originals::EOSProcessRequest);
                        break;
                    }
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
            std::wstring newurl = BackendURL;

            if (*endpoint != L'/') newurl += L'/';
            newurl += endpoint;

            HttpRequest->SetURL(newurl.c_str());
        }
    }

	return Originals::ProcessRequest(HttpRequest);
}

void Request::EOSProcessRequest(Containers::FCurlHttpRequest* HttpRequest)
{
    auto URL = HttpRequest->GetURL().ToString();
    const wchar_t* domains[] = { L"epicgames.com", L"epicgames.dev", L"epicgames.net" };

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
            std::wstring newurl = BackendURL;

            if (*endpoint != L'/') newurl += L'/';
            newurl += endpoint;

            std::wcout << L"NEWURL: " << newurl << std::endl;

            HttpRequest->SetURL(newurl.c_str());
        }
    }

    return Originals::EOSProcessRequest(HttpRequest);
}


void Request::Patch()
{
    auto sRef = Finder::FindString(L"STAT_FCurlHttpRequest_ProcessRequest", ImageBase);
    for (int i = 0; i < 2048; i++)
    {
        if (*(uint8_t*)(sRef - i) == 0x4C && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xDC)
        {
            Hook(sRef - i, ProcessRequest, (void**)&Originals::ProcessRequest);
            break;
        }
        else if (*(uint8_t*)(sRef - i) == 0x48 && *(uint8_t*)(sRef - i + 1) == 0x8B && *(uint8_t*)(sRef - i + 2) == 0xC4)
        {
            Hook(sRef - i, ProcessRequest, (void**)&Originals::ProcessRequest);
            break;
        }
    }
}