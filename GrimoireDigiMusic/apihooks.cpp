#include <Windows.h>

#include "apihooks.h"

#include <detours.h>
#include "myDMLoader.h"
#include "myDMPerformance.h"
#include "logger.h"

#include "globals.h"

HRESULT WINAPI CoCreateInstance_mine(__in REFCLSID rclsid, __in LPUNKNOWN pUnkOuter,
    __in DWORD dwClsContext, __in REFIID riid, __out LPVOID *ppv);

static HRESULT(WINAPI * CoCreateInstance_real)(__in REFCLSID rclsid, __in LPUNKNOWN pUnkOuter,
    __in DWORD dwClsContext, __in REFIID riid,
    __out  LPVOID *ppv) = CoCreateInstance;

HWND WINAPI CreateWindowExA_mine(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
    DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
    HINSTANCE hInstance, LPVOID lpParam);

static HWND (WINAPI * CreateWindowExA_real)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, 
    DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, 
    HINSTANCE hInstance, LPVOID lpParam) = CreateWindowExA;


HRESULT WINAPI CoCreateInstance_mine(__in REFCLSID rclsid, __in LPUNKNOWN pUnkOuter, __in DWORD dwClsContext,
    __in REFIID riid, __out LPVOID *ppv)
{
    if (rclsid == CLSID_DirectMusicPerformance && riid == IID_IDirectMusicPerformance)
    {
        *ppv = new myDMPerformance;
    }
    else if (rclsid == CLSID_DirectMusicLoader && riid == IID_IDirectMusicLoader)
    {
        *ppv = new myDMLoader;
    }
    else
    {
        return CoCreateInstance_real(rclsid, pUnkOuter, dwClsContext, riid, ppv);
    }

    return S_OK;
}

HWND WINAPI CreateWindowExA_mine(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
    DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
    HINSTANCE hInstance, LPVOID lpParam)
{
    //std::string newWndName = std::string(lpWindowName) + "  |  DigiMusic";    

    //g_hWnd = CreateWindowExA_real(dwExStyle, lpClassName, newWndName.c_str(), dwStyle, X, Y, nWidth, 
    //    nHeight, hWndParent, hMenu, hInstance, lpParam);

    g_hWnd = CreateWindowExA_real(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth,
        nHeight, hWndParent, hMenu, hInstance, lpParam);
    
    return g_hWnd;
}

void InstallAPIHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)CoCreateInstance_real, CoCreateInstance_mine);
    DetourAttach(&(PVOID&)CreateWindowExA_real, CreateWindowExA_mine);
    DetourTransactionCommit();
}

void RemoveAPIHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)CoCreateInstance_real, CoCreateInstance_mine);
    DetourDetach(&(PVOID&)CreateWindowExA_real, CreateWindowExA_mine);
    DetourTransactionCommit();
}

