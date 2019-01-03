#include <Windows.h>

#include "myDMPerformance.h"
#include "myDMSegment.h"

#include "logger.h"
#include "globals.h"
#include "file.h"
#include <bass.h>
#include <stdio.h>

/*
CDXMidi::Shutdown  (Release, CloseDown)
CDXMidi::Init      (Init, AddPort)
CDXMidi::Play      (PlaySegment)
CDXMidi::Stop      (Stop)
CDXMidi::IsPlaying (IsPlaying)
CDXMidi::SetVolume (SetGlobalParam)
*/

/************************************************************************************************************
 * ULONG __stdcall myDMPerformance::Release
 *
 * Wrapper for:
 *      IDirectMusicPerformance::Release
 *
 * Called from:
 *      CDXMidi::Shutdown
 ***********************************************************************************************************/

ULONG __stdcall myDMPerformance::Release()
{
    LOGFUNCTIONCALL;

    delete this;

    return 0;
}

/************************************************************************************************************
 * HRESULT __stdcall myDMPerformance::Init 
 *
 * Wrapper for:
 *      IDirectMusicPerformance::Init
 *
 * Called from: 
 *      CDXMidi::Init
 *
 * Notes:
 *      Returning error (any negative value) from this function will shut down Grimoire's music subsystem
 *      The game will continue without music and no log entry will be created
 ***********************************************************************************************************/

HRESULT __stdcall myDMPerformance::Init(IDirectMusic **ppDirectMusic, LPDIRECTSOUND pDirectSound, HWND hWnd)
{
    LOGFUNCTIONCALL;

    //  Initialize audio library

    if (!BASS_Init(-1, 44100, 0, g_hWnd, NULL))
    {
        LOG_ERROR("BASS_Init failed with error code: ", BASS_ErrorGetCode());
        return ~S_OK;
    }

    // Check if music folder exists

    if (DirExists("Music") == false)
    {
        LOG_ERROR("Music folder not found");
        return ~S_OK;
    }

    // Increase BASS playback buffer length to reduce the chance of sound breaking up on slow computers
 
    if (!BASS_SetConfig(BASS_CONFIG_BUFFER, 1000))
    {
        LOG_ERROR("Unable to set playback buffer length, BASS_SetConfig failed with error code: ", BASS_ErrorGetCode());
        return ~S_OK;
    }

    return S_OK;
}

// CDXMidi::Play

HRESULT __stdcall myDMPerformance::PlaySegment(IDirectMusicSegment *pSegment, DWORD dwFlags, __int64 i64StartTime, IDirectMusicSegmentState **ppSegmentState)
{
    LOGFUNCTIONCALL;

    if (!BASS_ChannelPlay(dynamic_cast<myDMSegment*>(pSegment)->m_hStream, TRUE))
    {
        LOG_ERROR("BASS_ChannelPlay failed with error code: ", BASS_ErrorGetCode());
        return ~S_OK;
    }

    return S_OK;
}

// CDXMidi::Stop

HRESULT __stdcall myDMPerformance::Stop(IDirectMusicSegment *pSegment, IDirectMusicSegmentState* pSegmentState, MUSIC_TIME mtTime, DWORD dwFlags)
{
    LOGFUNCTIONCALL;

    if (!BASS_ChannelStop(dynamic_cast<myDMSegment*>(pSegment)->m_hStream))
    {
        LOG_ERROR("BASS_ChannelStop failed with error code: ", BASS_ErrorGetCode());
    }

    return S_OK;
}

// CDXMidi::IsPlaying

HRESULT __stdcall myDMPerformance::IsPlaying(IDirectMusicSegment* pSegment, IDirectMusicSegmentState* pSegState)
{
//#ifdef DEBUGLOG_ISPLAYING
//    LOGFUNCTIONCALL
//#endif

    return (BASS_ChannelIsActive(dynamic_cast<myDMSegment*>(pSegment)->m_hStream) == BASS_ACTIVE_PLAYING) ? S_OK : S_FALSE;
}

// CDXMidi::Init

HRESULT __stdcall myDMPerformance::AddPort(IDirectMusicPort* pPort)
{
    LOGFUNCTIONCALL;

    return S_OK;
}

// CDXMidi::SetVolume

HRESULT __stdcall myDMPerformance::SetGlobalParam(REFGUID rguidType, void* pParam, DWORD dwSize)
{
    LOGFUNCTIONCALL;

    if (rguidType != GUID_PerfMasterVolume)
    {
        char buf[36 + 1];

        sprintf_s(buf, "%08lx-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
            rguidType.Data1, rguidType.Data2, rguidType.Data3,
            rguidType.Data4[0], rguidType.Data4[1], rguidType.Data4[2], rguidType.Data4[3],
            rguidType.Data4[4], rguidType.Data4[5], rguidType.Data4[6], rguidType.Data4[7]);

        LOG_CRITICAL("Unexpected GUID: ", buf);

        return ~S_OK;
    }

    // BASS stream global volume level range is from 0 (silent) to 10000 (full)

    DWORD streamVol;

    if (*(int*)pParam <= -10000)
    {
        streamVol = 0;
    }
    else
    { 
        streamVol = (DWORD)((double)(3000.0 - abs(*(int*)pParam)) * (10000.0 / 3000.0));
    }

    if (!BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, streamVol))
    {
        LOG_ERROR("Unable to set volume. BASS_SetConfig failed with error code: ", BASS_ErrorGetCode());
    }

    return S_OK;
}

// CDXMidi::Shutdown

HRESULT __stdcall myDMPerformance::CloseDown()
{
    LOGFUNCTIONCALL;

    if (!BASS_Free())
    {
        LOG_ERROR("BASS_Free failed with error code: ", BASS_ErrorGetCode());
    }

    return S_OK; // The method returns S_OK
}

// Destructor

myDMPerformance::~myDMPerformance() 
{
    LOGFUNCTIONCALL;
}


/******************************************************************************/
/*                                                                            */
/*                      Methods not used in Grimoire                          */
/*                                                                            */
/******************************************************************************/


HRESULT __stdcall myDMPerformance::QueryInterface(const IID& riid, LPVOID *obp)
{
    UNEXPECTEDMETHODCALL;
}

ULONG __stdcall myDMPerformance::AddRef()
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetSegmentState(IDirectMusicSegmentState **ppSegmentState, MUSIC_TIME mtTime)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::SetPrepareTime(DWORD dwMilliSeconds)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetPrepareTime(DWORD* pdwMilliSeconds)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::SetBumperLength(DWORD dwMilliSeconds)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetBumperLength(DWORD* pdwMilliSeconds)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::SendPMsg(DMUS_PMSG* pPMSG)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::MusicToReferenceTime(MUSIC_TIME mtTime, REFERENCE_TIME* prtTime)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::ReferenceToMusicTime(REFERENCE_TIME rtTime, MUSIC_TIME* pmtTime)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetTime(REFERENCE_TIME* prtNow, MUSIC_TIME* pmtNow)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::AllocPMsg(ULONG cb, DMUS_PMSG** ppPMSG)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::FreePMsg(DMUS_PMSG* pPMSG)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetGraph(IDirectMusicGraph** ppGraph)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::SetGraph(IDirectMusicGraph* pGraph)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::SetNotificationHandle(HANDLE hNotification, REFERENCE_TIME rtMinimum)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetNotificationPMsg(DMUS_NOTIFICATION_PMSG** ppNotificationPMsg)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::AddNotificationType(REFGUID rguidNotificationType)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::RemoveNotificationType(REFGUID rguidNotificationType)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::RemovePort(IDirectMusicPort* pPort)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::AssignPChannelBlock(DWORD dwBlockNum, IDirectMusicPort* pPort, DWORD dwGroup)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::AssignPChannel(DWORD dwPChannel, IDirectMusicPort* pPort, DWORD dwGroup, DWORD dwMChannel)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::PChannelInfo(DWORD dwPChannel, IDirectMusicPort** ppPort, DWORD* pdwGroup, DWORD* pdwMChannel)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::DownloadInstrument(IDirectMusicInstrument* pInst, DWORD dwPChannel, IDirectMusicDownloadedInstrument** ppDownInst, DMUS_NOTERANGE* pNoteRanges, DWORD dwNumNoteRanges, IDirectMusicPort** ppPort, DWORD* pdwGroup, DWORD* pdwMChannel)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::Invalidate(MUSIC_TIME mtTime, DWORD dwFlags)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, MUSIC_TIME* pmtNext, void* pParam)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::SetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, void* pParam)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetGlobalParam(REFGUID rguidType, void* pParam, DWORD dwSize)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetLatencyTime(REFERENCE_TIME* prtTime)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetQueueTime(REFERENCE_TIME* prtTime)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::AdjustTime(REFERENCE_TIME rtAmount)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::GetResolvedTime(REFERENCE_TIME rtTime, REFERENCE_TIME* prtResolved, DWORD dwTimeResolveFlags)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::MIDIToMusic(BYTE bMIDIValue, DMUS_CHORD_KEY* pChord, BYTE bPlayMode, BYTE bChordLevel, WORD *pwMusicValue)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::MusicToMIDI(WORD wMusicValue, DMUS_CHORD_KEY* pChord, BYTE bPlayMode, BYTE bChordLevel, BYTE *pbMIDIValue)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::TimeToRhythm(MUSIC_TIME mtTime, DMUS_TIMESIGNATURE *pTimeSig, WORD *pwMeasure, BYTE *pbBeat, BYTE *pbGrid, short *pnOffset)
{
    UNEXPECTEDMETHODCALL;
}

HRESULT __stdcall myDMPerformance::RhythmToTime(WORD wMeasure, BYTE bBeat, BYTE bGrid, short nOffset, DMUS_TIMESIGNATURE *pTimeSig, MUSIC_TIME *pmtTime)
{
    UNEXPECTEDMETHODCALL;
}
