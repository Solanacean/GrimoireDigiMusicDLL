#pragma once

#include <dmusici.h>

class myDMPerformance : IDirectMusicPerformance
{
    // IUnknown methods
    HRESULT __stdcall QueryInterface(const IID& riid, LPVOID *obp) final;
    ULONG   __stdcall AddRef() final;
    ULONG   __stdcall Release() final;

    // IDirectMusicPerformance methods
    HRESULT __stdcall Init(IDirectMusic **ppDirectMusic, LPDIRECTSOUND pDirectSound, HWND hWnd) final;
    HRESULT __stdcall PlaySegment(IDirectMusicSegment *pSegment, DWORD dwFlags, __int64 i64StartTime, IDirectMusicSegmentState **ppSegmentState) final;
    HRESULT __stdcall Stop(IDirectMusicSegment *pSegment, IDirectMusicSegmentState* pSegmentState, MUSIC_TIME mtTime, DWORD dwFlags) final;
    HRESULT __stdcall GetSegmentState(IDirectMusicSegmentState **ppSegmentState, MUSIC_TIME mtTime) final;
    HRESULT __stdcall SetPrepareTime(DWORD dwMilliSeconds) final;
    HRESULT __stdcall GetPrepareTime(DWORD* pdwMilliSeconds) final;
    HRESULT __stdcall SetBumperLength(DWORD dwMilliSeconds) final;
    HRESULT __stdcall GetBumperLength(DWORD* pdwMilliSeconds) final;
    HRESULT __stdcall SendPMsg(DMUS_PMSG* pPMSG) final;
    HRESULT __stdcall MusicToReferenceTime(MUSIC_TIME mtTime, REFERENCE_TIME* prtTime) final;
    HRESULT __stdcall ReferenceToMusicTime(REFERENCE_TIME rtTime, MUSIC_TIME* pmtTime) final;
    HRESULT __stdcall IsPlaying(IDirectMusicSegment* pSegment, IDirectMusicSegmentState* pSegState) final;
    HRESULT __stdcall GetTime(REFERENCE_TIME* prtNow, MUSIC_TIME* pmtNow) final;
    HRESULT __stdcall AllocPMsg(ULONG cb, DMUS_PMSG** ppPMSG) final;
    HRESULT __stdcall FreePMsg(DMUS_PMSG* pPMSG) final;
    HRESULT __stdcall GetGraph(IDirectMusicGraph** ppGraph) final;
    HRESULT __stdcall SetGraph(IDirectMusicGraph* pGraph) final;
    HRESULT __stdcall SetNotificationHandle(HANDLE hNotification, REFERENCE_TIME rtMinimum) final;
    HRESULT __stdcall GetNotificationPMsg(DMUS_NOTIFICATION_PMSG** ppNotificationPMsg) final;
    HRESULT __stdcall AddNotificationType(REFGUID rguidNotificationType) final;
    HRESULT __stdcall RemoveNotificationType(REFGUID rguidNotificationType) final;
    HRESULT __stdcall AddPort(IDirectMusicPort* pPort) final;
    HRESULT __stdcall RemovePort(IDirectMusicPort* pPort) final;
    HRESULT __stdcall AssignPChannelBlock(DWORD dwBlockNum, IDirectMusicPort* pPort, DWORD dwGroup) final;
    HRESULT __stdcall AssignPChannel(DWORD dwPChannel, IDirectMusicPort* pPort, DWORD dwGroup, DWORD dwMChannel) final;
    HRESULT __stdcall PChannelInfo(DWORD dwPChannel, IDirectMusicPort** ppPort, DWORD* pdwGroup, DWORD* pdwMChannel) final;
    HRESULT __stdcall DownloadInstrument(IDirectMusicInstrument* pInst, DWORD dwPChannel, IDirectMusicDownloadedInstrument** ppDownInst, DMUS_NOTERANGE* pNoteRanges, DWORD dwNumNoteRanges, IDirectMusicPort** ppPort, DWORD* pdwGroup, DWORD* pdwMChannel) final;
    HRESULT __stdcall Invalidate(MUSIC_TIME mtTime, DWORD dwFlags) final;
    HRESULT __stdcall GetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, MUSIC_TIME* pmtNext, void* pParam) final;
    HRESULT __stdcall SetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, void* pParam) final;
    HRESULT __stdcall GetGlobalParam(REFGUID rguidType, void* pParam, DWORD dwSize) final;
    HRESULT __stdcall SetGlobalParam(REFGUID rguidType, void* pParam, DWORD dwSize) final;
    HRESULT __stdcall GetLatencyTime(REFERENCE_TIME* prtTime) final;
    HRESULT __stdcall GetQueueTime(REFERENCE_TIME* prtTime) final;
    HRESULT __stdcall AdjustTime(REFERENCE_TIME rtAmount) final;
    HRESULT __stdcall CloseDown() final;
    HRESULT __stdcall GetResolvedTime(REFERENCE_TIME rtTime, REFERENCE_TIME* prtResolved, DWORD dwTimeResolveFlags) final;
    HRESULT __stdcall MIDIToMusic(BYTE bMIDIValue, DMUS_CHORD_KEY* pChord, BYTE bPlayMode, BYTE bChordLevel, WORD *pwMusicValue) final;
    HRESULT __stdcall MusicToMIDI(WORD wMusicValue, DMUS_CHORD_KEY* pChord, BYTE bPlayMode, BYTE bChordLevel, BYTE *pbMIDIValue) final;
    HRESULT __stdcall TimeToRhythm(MUSIC_TIME mtTime, DMUS_TIMESIGNATURE *pTimeSig, WORD *pwMeasure, BYTE *pbBeat, BYTE *pbGrid, short *pnOffset) final;
    HRESULT __stdcall RhythmToTime(WORD wMeasure, BYTE bBeat, BYTE bGrid, short nOffset, DMUS_TIMESIGNATURE *pTimeSig, MUSIC_TIME *pmtTime) final;

    virtual ~myDMPerformance();
};
