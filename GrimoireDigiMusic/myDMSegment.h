#pragma once

#include <Windows.h>

#include <dmusici.h>
#include <bass.h>
#include <atomic>

class myDMSegment : public IDirectMusicSegment
{

public:
    HSTREAM m_hStream;
    std::atomic<HRESULT> m_isPlaying {S_FALSE};

    myDMSegment(HSTREAM hStream) : m_hStream(hStream) {}

private:
    // IUnknown methods
    HRESULT __stdcall QueryInterface(const IID& riid, LPVOID *obp) final;
    ULONG   __stdcall AddRef() final;
    ULONG   __stdcall Release() final;

    // IDirectMusicSegment methods
    HRESULT __stdcall GetLength(MUSIC_TIME* pmtLength) final;
    HRESULT __stdcall SetLength(MUSIC_TIME mtLength) final;
    HRESULT __stdcall GetRepeats(DWORD* pdwRepeats) final;
    HRESULT __stdcall SetRepeats(DWORD  dwRepeats) final;
    HRESULT __stdcall GetDefaultResolution(DWORD* pdwResolution) final;
    HRESULT __stdcall SetDefaultResolution(DWORD  dwResolution) final;
    HRESULT __stdcall GetTrack(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, IDirectMusicTrack** ppTrack) final;
    HRESULT __stdcall GetTrackGroup(IDirectMusicTrack* pTrack, DWORD* pdwGroupBits) final;
    HRESULT __stdcall InsertTrack(IDirectMusicTrack* pTrack, DWORD dwGroupBits) final;
    HRESULT __stdcall RemoveTrack(IDirectMusicTrack* pTrack) final;
    HRESULT __stdcall InitPlay(IDirectMusicSegmentState** ppSegState, IDirectMusicPerformance* pPerformance, DWORD dwFlags) final;
    HRESULT __stdcall GetGraph(IDirectMusicGraph** ppGraph) final;
    HRESULT __stdcall SetGraph(IDirectMusicGraph* pGraph) final;
    HRESULT __stdcall AddNotificationType(REFGUID rguidNotificationType) final;
    HRESULT __stdcall RemoveNotificationType(REFGUID rguidNotificationType) final;
    HRESULT __stdcall GetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, MUSIC_TIME* pmtNext, void* pParam) final;
    HRESULT __stdcall SetParam(REFGUID rguidType, DWORD dwGroupBits, DWORD dwIndex, MUSIC_TIME mtTime, void* pParam) final;
    HRESULT __stdcall Clone(MUSIC_TIME mtStart, MUSIC_TIME mtEnd, IDirectMusicSegment** ppSegment) final;
    HRESULT __stdcall SetStartPoint(MUSIC_TIME mtStart) final;
    HRESULT __stdcall GetStartPoint(MUSIC_TIME* pmtStart) final;
    HRESULT __stdcall SetLoopPoints(MUSIC_TIME mtStart, MUSIC_TIME mtEnd) final;
    HRESULT __stdcall GetLoopPoints(MUSIC_TIME* pmtStart, MUSIC_TIME* pmtEnd) final;
    HRESULT __stdcall SetPChannelsUsed(DWORD dwNumPChannels, DWORD* paPChannels) final;

    virtual ~myDMSegment();
};

