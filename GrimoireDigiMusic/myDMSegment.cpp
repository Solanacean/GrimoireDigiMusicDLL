#include <Windows.h>

#include "myDMSegment.h"

#include "logger.h"
#include <bass.h>

/*
CDXMidi::LoadMidi (SetParam)
CDXMidi::Remove   (Release, SetParam)
*/

// CDXMidi::Remove

ULONG __stdcall myDMSegment::Release()
{
    LOGFUNCTIONCALL;

    if (!BASS_StreamFree(m_hStream))
    {      
        LOG_ERROR("BASS_StreamFree failed with error code: ", BASS_ErrorGetCode());
    }

    delete this;

    return S_OK;
}

// CDXMidi::LoadMidi, CDXMidi::Remove

HRESULT __stdcall myDMSegment::SetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, void* pParam)
{
    LOGFUNCTIONCALL;

    return S_OK; 
}

// Destructor

myDMSegment::~myDMSegment() 
{
    LOGFUNCTIONCALL;
}


/******************************************************************************/
/*                                                                            */
/*                      Methods not used in Grimoire                          */
/*                                                                            */
/******************************************************************************/


HRESULT __stdcall myDMSegment::QueryInterface(const IID& riid, LPVOID *obp)
{
    UNEXPECTEDMETHODCALL;
}

ULONG __stdcall myDMSegment::AddRef()
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetLength(MUSIC_TIME* pmtLength)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetLength(MUSIC_TIME mtLength)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetRepeats(DWORD* pdwRepeats)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetRepeats(DWORD  dwRepeats)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetDefaultResolution(DWORD* pdwResolution)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetDefaultResolution(DWORD  dwResolution)
{
    UNEXPECTEDMETHODCALL;
}
HRESULT __stdcall myDMSegment::GetTrack(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, IDirectMusicTrack** ppTrack)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetTrackGroup(IDirectMusicTrack* pTrack, DWORD* pdwGroupBits)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::InsertTrack(IDirectMusicTrack* pTrack, DWORD dwGroupBits)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::RemoveTrack(IDirectMusicTrack* pTrack)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::InitPlay(IDirectMusicSegmentState** ppSegState, IDirectMusicPerformance* pPerformance, DWORD dwFlags)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetGraph(IDirectMusicGraph** ppGraph)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetGraph(IDirectMusicGraph* pGraph)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::AddNotificationType(REFGUID rguidNotificationType)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::RemoveNotificationType(REFGUID rguidNotificationType)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, MUSIC_TIME* pmtNext, void* pParam)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::Clone(MUSIC_TIME mtStart, MUSIC_TIME mtEnd, IDirectMusicSegment** ppSegment)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetStartPoint(MUSIC_TIME mtStart)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetStartPoint(MUSIC_TIME* pmtStart)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetLoopPoints(MUSIC_TIME mtStart, MUSIC_TIME mtEnd)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::GetLoopPoints(MUSIC_TIME* pmtStart, MUSIC_TIME* pmtEnd)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMSegment::SetPChannelsUsed(DWORD dwNumPChannels, DWORD* paPChannels)
{
    UNEXPECTEDMETHODCALL;
}
