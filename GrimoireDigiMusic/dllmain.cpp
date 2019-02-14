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
        Logger::Instance().Init("DIGIMUSIC_LOG.txt", Logger::Level::warning);

        {
            TCHAR szPath[MAX_PATH];
            GetSystemDirectory(szPath, _countof(szPath));
            strcat_s(szPath, _countof(szPath), "\\dsound.dll");
            DWORD dwAttrib = GetFileAttributes(szPath);
            if (dwAttrib == INVALID_FILE_ATTRIBUTES || dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
            {
                LOG_FATAL("System dsound.dll is missing. If you're using WINE, install the native dsound.dll using Winetricks and configure WINE to use the installed version instead of the built-in one.");
                return FALSE;
            }
        }

        InstallAPIHooks(); 
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
