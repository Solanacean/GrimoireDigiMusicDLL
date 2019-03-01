#include <Windows.h>

#include "myDMPerformance.h"
#include "myDMSegment.h"

#include "logger.h"
#include "globals.h"
#include "file.h"
#include <bass.h>
#include <stdio.h>
#include <atomic>

#define GRIMVOL_TO_DMUSVOL(volume) (-30 * (100 - volume))

#define BASS_STREAM_VOLUME_MIN 0
#define BASS_STREAM_VOLUME_MAX 10000
#define GRIM_VOLUME_MIN 0
#define GRIM_VOLUME_MAX 100
#define GRIM_DMUS_VOLUME_NONCOMP_MIN GRIMVOL_TO_DMUSVOL(GRIM_VOLUME_MIN)
#define GRIM_DMUS_VOLUME_MAX GRIMVOL_TO_DMUSVOL(GRIM_VOLUME_MAX)

inline double ConvertRange(double value, double oldStart, double oldEnd, double newStart, double newEnd)
{
    double oldRangeLength = oldEnd - oldStart;
    if (!oldRangeLength)
    {
        return newStart;
    }
    double scale = (newEnd - newStart) / oldRangeLength;
    return (newStart + ((value - oldStart) * scale));
}

void CALLBACK MySyncProc(HSYNC handle, DWORD channel, DWORD data, myDMSegment *pSeg)
{
    pSeg->m_isPlaying.store(S_FALSE, std::memory_order_relaxed);
}

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

    return S_OK;
}

// CDXMidi::Play

HRESULT __stdcall myDMPerformance::PlaySegment(IDirectMusicSegment *pSegment, DWORD dwFlags, __int64 i64StartTime, IDirectMusicSegmentState **ppSegmentState)
{
    LOGFUNCTIONCALL;

    auto *pSeg = static_cast<myDMSegment*>(pSegment);

    if (!BASS_ChannelSetSync(pSeg->m_hStream, BASS_SYNC_END | BASS_SYNC_ONETIME, 0, (SYNCPROC *)MySyncProc, pSeg))
    {
        LOG_ERROR("BASS_ChannelSetSync failed with error code: ", BASS_ErrorGetCode());
        return ~S_OK;
    }

    pSeg->m_isPlaying.store(S_OK, std::memory_order_relaxed);

    if (!BASS_ChannelPlay(pSeg->m_hStream, TRUE))
    {
        pSeg->m_isPlaying.store(S_FALSE, std::memory_order_relaxed);

        LOG_ERROR("BASS_ChannelPlay failed with error code: ", BASS_ErrorGetCode());
        return ~S_OK;
    }

    return S_OK;
}

// CDXMidi::Stop

HRESULT __stdcall myDMPerformance::Stop(IDirectMusicSegment *pSegment, IDirectMusicSegmentState* pSegmentState, MUSIC_TIME mtTime, DWORD dwFlags)
{
    LOGFUNCTIONCALL;

    auto *pSeg = static_cast<myDMSegment*>(pSegment);

    if (!BASS_ChannelStop(pSeg->m_hStream))
    {
        LOG_ERROR("BASS_ChannelStop failed with error code: ", BASS_ErrorGetCode());
    }
    else
    {
        pSeg->m_isPlaying.store(S_FALSE, std::memory_order_relaxed);
    }

    return S_OK;
}

// CDXMidi::IsPlaying

HRESULT __stdcall myDMPerformance::IsPlaying(IDirectMusicSegment* pSegment, IDirectMusicSegmentState* pSegState)
{
    auto *pSeg = static_cast<myDMSegment*>(pSegment);
 
    return pSeg->m_isPlaying.load(std::memory_order_relaxed);
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

    auto perfMasterVol = *static_cast<int32_t*>(pParam);

    if (perfMasterVol < GRIM_DMUS_VOLUME_NONCOMP_MIN)
    {
        perfMasterVol = GRIM_DMUS_VOLUME_NONCOMP_MIN;
    }

    DWORD streamVol = (DWORD)ConvertRange(perfMasterVol, 
        GRIM_DMUS_VOLUME_NONCOMP_MIN, GRIM_DMUS_VOLUME_MAX, BASS_STREAM_VOLUME_MIN, BASS_STREAM_VOLUME_MAX);

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

