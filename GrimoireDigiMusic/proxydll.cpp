#include <Windows.h>

#include "proxydll.h"

#include <tchar.h>

//#include "logger.h"

struct exportRec_t
{
    FARPROC addr;
    LPCSTR name;
};

static exportRec_t exportRecDsound[] = {
    {NULL, "DirectSoundCreate"},
    {NULL, "DirectSoundEnumerateA"},
    {NULL, "DirectSoundEnumerateW"},
    {NULL, "DllCanUnloadNow"},
    {NULL, "DllGetClassObject"},
    {NULL, "DirectSoundCaptureCreate"},
    {NULL, "DirectSoundCaptureEnumerateA"},
    {NULL, "DirectSoundCaptureEnumerateW"},
    {NULL, "GetDeviceID"},
    {NULL, "DirectSoundFullDuplexCreate"},
    {NULL, "DirectSoundCreate8"},
    {NULL, "DirectSoundCaptureCreate8"}
};

static HMODULE hmodDsound = 0;

FARPROC __stdcall ResolveExport(size_t index);

bool LoadRealDsound()
{
    TCHAR path[MAX_PATH];

    if (!hmodDsound)
    {
        GetSystemDirectory(path, _countof(path));
        _tcscat_s(path, _countof(path), _T("\\dsound.dll"));
        hmodDsound = LoadLibrary(path);
    }

    return hmodDsound ? true : false;
}

bool FreeRealDsound()
{
    bool result = false;

    if (!hmodDsound)
        return result;

    if (FreeLibrary(hmodDsound))
    {
        hmodDsound = NULL;
        result = true;
    }

    return result;
}

FARPROC __stdcall ResolveExport(size_t index)
{
    if (index < _countof(exportRecDsound))
    {
        if (hmodDsound && exportRecDsound[index].addr)
            return exportRecDsound[index].addr;

        if (LoadRealDsound())
            return exportRecDsound[index].addr = GetProcAddress(hmodDsound, exportRecDsound[index].name);
    }
    return NULL;
}

#define PROXY_BODY(id)\
_asm push id \
_asm call ResolveExport \
_asm jmp eax

__declspec(naked) void DirectSoundCreate_proxy()            { PROXY_BODY(0) }
__declspec(naked) void DirectSoundEnumerateA_proxy()        { PROXY_BODY(1) }
__declspec(naked) void DirectSoundEnumerateW_proxy()        { PROXY_BODY(2) }
__declspec(naked) void DllCanUnloadNow_proxy()              { PROXY_BODY(3) }
__declspec(naked) void DllGetClassObject_proxy()            { PROXY_BODY(4) }
__declspec(naked) void DirectSoundCaptureCreate_proxy()     { PROXY_BODY(5) }
__declspec(naked) void DirectSoundCaptureEnumerateA_proxy() { PROXY_BODY(6) }
__declspec(naked) void DirectSoundCaptureEnumerateW_proxy() { PROXY_BODY(7) }
__declspec(naked) void GetDeviceID_proxy()                  { PROXY_BODY(8) }
__declspec(naked) void DirectSoundFullDuplexCreate_proxy()  { PROXY_BODY(9) }
__declspec(naked) void DirectSoundCreate8_proxy()           { PROXY_BODY(10) }
__declspec(naked) void DirectSoundCaptureCreate8_proxy()    { PROXY_BODY(11) }
