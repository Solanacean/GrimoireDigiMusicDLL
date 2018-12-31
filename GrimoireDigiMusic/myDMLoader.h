#pragma once

#include <dmusici.h>

class myDMLoader : IDirectMusicLoader
{
    // IUnknown methods
    HRESULT __stdcall QueryInterface(const IID& riid, LPVOID *obp) final;
    ULONG   __stdcall AddRef() final;
    ULONG   __stdcall Release() final;

    // IDirectMusicLoader methods
    HRESULT __stdcall GetObject(LPDMUS_OBJECTDESC pDesc, REFIID riid, LPVOID FAR *ppv) final;
    HRESULT __stdcall SetObject(LPDMUS_OBJECTDESC pDesc) final;
    HRESULT __stdcall SetSearchDirectory(REFGUID rguidClass, WCHAR *pwzPath, BOOL fClear) final;
    HRESULT __stdcall ScanDirectory(REFGUID rguidClass, WCHAR *pwzFileExtension, WCHAR *pwzScanFileName) final;
    HRESULT __stdcall CacheObject(IDirectMusicObject * pObject) final;
    HRESULT __stdcall ReleaseObject(IDirectMusicObject * pObject) final;
    HRESULT __stdcall ClearCache(REFGUID rguidClass) final;
    HRESULT __stdcall EnableCache(REFGUID rguidClass, BOOL fEnable) final;
    HRESULT __stdcall EnumObject(REFGUID rguidClass, DWORD dwIndex, LPDMUS_OBJECTDESC pDesc) final;

    virtual ~myDMLoader();
};


