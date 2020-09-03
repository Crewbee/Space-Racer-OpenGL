#pragma once
#include <roapi.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

using namespace std;

class MediaStreamer
{
public:
	MediaStreamer(_In_ Platform::StringReference* url);

	void Restart();
	vector<byte> ReadAll();
	vector<byte> GetNextBuffer();

	const WAVEFORMATEX& GetOutputWaveFormatEx() const
	{
		return m_waveFormat;
	}

	uint32 GetMaxStreamLengthInBytes() const
	{
		return m_maxStreamLengthInBytes;
	}

private:
	Microsoft::WRL::ComPtr<IMFSourceReader> m_reader;
	WAVEFORMATEX                            m_waveFormat;
	uint32                                  m_maxStreamLengthInBytes;
};
