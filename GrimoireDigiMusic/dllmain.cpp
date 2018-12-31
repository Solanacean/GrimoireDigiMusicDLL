#include "targetver.h"

// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>

#include <tchar.h>
#include <detours.h>
#include "proxydll.h"
#include "apihooks.h"
#include "logger.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        //DetourRestoreAfterWith();
        InstallAPIHooks();
        Logger::Instance().Init("DIGIMUSIC_LOG.txt", Logger::Level::warning);
        
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        RemoveAPIHooks();
        //FreeRealDsound();
        break;
    }

    return TRUE;
}
