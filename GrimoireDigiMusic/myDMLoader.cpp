#include <Windows.h>

#include "myDMLoader.h"
#include "myDMSegment.h"

#include "file.h"
#include "logger.h"
#include <bass.h>


/*
CDXMidi::LoadMidi (GetObject, ClearCache)
CDXMidi::Shutdown (Release)
*/

// CDXMidi::LoadMidi()

HRESULT __stdcall myDMLoader::GetObject(LPDMUS_OBJECTDESC pDesc, REFIID riid, LPVOID FAR *ppv)
{
    LOGFUNCTIONCALL;

    std::string name_mp3;
    std::string path_mp3;

    HSTREAM hStream = 0;

    int midiLen = static_cast<int>(pDesc->llMemLength);

    name_mp3 = MIDILenToFileName(midiLen);

    if (!name_mp3.empty())
    {
        path_mp3 = "Music\\" + name_mp3;
        
        //LOG_DEBUG("Creating stream from: ", path_mp3);

        hStream = BASS_StreamCreateFile(FALSE, path_mp3.c_str(), 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE);
        if (!hStream)
        {          
            LOG_ERROR("BASS_StreamCreateFile failed to create stream from: ", path_mp3, ". Error code: ", BASS_ErrorGetCode());
            //return ~S_OK;
        }
        //*ppv = new myDMSegment(hStream);
    }
    else
    {
        LOG_ERROR("Map key not found: ", midiLen);
        //*ppv = new myDMSegment();
        //return ~S_OK;
    }

    *ppv = new myDMSegment(hStream);

    return S_OK;

    // NOTE: returning value indicating an error (any negative value such as ~S_OK) makes 
    // Grimoire complain about failure to load the corresponding .mid file and terminate.
    // To prevent this, we return value indicating success (S_OK) even in case of an error.

}

// CDXMidi::LoadMidi()

HRESULT __stdcall myDMLoader::ClearCache(REFGUID rguidClass)
{
    LOGFUNCTIONCALL;

    return S_OK;
}

// CDXMidi::Shutdown()

ULONG __stdcall myDMLoader::Release()
{
    LOGFUNCTIONCALL;

    delete this;

    return 0;
}

// Destructor

myDMLoader::~myDMLoader() 
{
    LOGFUNCTIONCALL;
}

/******************************************************************************/
/*                                                                            */
/*                      Methods not used in Grimoire                          */
/*                                                                            */
/******************************************************************************/


HRESULT __stdcall myDMLoader::QueryInterface(const IID& riid, LPVOID *obp)
{
    UNEXPECTEDMETHODCALL;
}

ULONG __stdcall myDMLoader::AddRef()
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::SetObject(LPDMUS_OBJECTDESC pDesc)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::SetSearchDirectory(REFGUID rguidClass, WCHAR *pwzPath, BOOL fClear)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::ScanDirectory(REFGUID rguidClass, WCHAR *pwzFileExtension, WCHAR *pwzScanFileName)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::CacheObject(IDirectMusicObject *pObject)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::ReleaseObject(IDirectMusicObject *pObject)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::EnableCache(REFGUID rguidClass, BOOL fEnable)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMLoader::EnumObject(REFGUID rguidClass, DWORD dwIndex, LPDMUS_OBJECTDESC pDesc)
{
    UNEXPECTEDMETHODCALL;
}
