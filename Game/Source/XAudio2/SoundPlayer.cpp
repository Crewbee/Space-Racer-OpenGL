//
// Copyright (c) 2017-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GamePCH.h"

#include "SoundPlayer.h"
#include "SoundChannel.h"
#include "SoundObject.h"
#include "Helpers/AudioController.h"

//====================================================================================================
// SoundPlayer
//====================================================================================================
SoundPlayer::SoundPlayer(PlayerType aType, SoundChannel** aSomeChannels)
{
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		m_pChannel.push_back(aSomeChannels[i]);
	}

	m_MyType = aType;
}

SoundPlayer::~SoundPlayer()
{
	m_pChannel.clear();
}

SoundObject* SoundPlayer::LoadSound(const char* fullpath, FXType aType)
{
	SoundObject* pSound = new SoundObject(aType);

	long len;
	const char* fileBuffer = LoadCompleteFile(fullpath, &len);
	assert(fileBuffer);
	if (fileBuffer)
	{
		MyWaveDescriptor waveDescriptor = WaveLoader::ParseWaveBuffer(fileBuffer, len);

		if (waveDescriptor.valid == false)
		{
			OutputMessage("WAV file parsing failed (%s)\n", fullpath);
			delete[] fileBuffer;
		}
		else
		{
			pSound->Init(fileBuffer, waveDescriptor);
		}
	}

	return pSound;
}

int SoundPlayer::PlaySound(SoundObject* pSoundObject, bool isPriority)
{
	if (pSoundObject->IsValid() == false)
		return -1; // Sound didn't play.
	if (m_MyType == PT_MUSIC)
	{
		m_pChannel[0]->PlaySound(pSoundObject);
		return 0;
	}
	else if (m_MyType == PT_MISC)
	{
		if (pSoundObject->GetType() == FXType::FX_TEMP)
		{
			int OldestChannel = 0;
			double ChannelAge = 0.0;
			for (int i = 0; i < 5; i++)
			{
				if (m_pChannel.at(i)->GetState() == SoundChannel::SoundChannelState_Free)
				{
					m_pChannel.at(i)->PlaySound(pSoundObject);
					return 0;
				}
				if (m_pChannel.at(i)->GetTimePlaybackStarted() > ChannelAge)
				{
					if (m_pChannel.at(i)->GetIsConst() != FXType::FX_CONST)
					{
						ChannelAge = m_pChannel.at(i)->GetTimePlaybackStarted();
						OldestChannel = i;
					}
				}

			}
			m_pChannel.at(OldestChannel)->StopSound();
			m_pChannel.at(OldestChannel)->PlaySound(pSoundObject);
			return 0;
		}
		else if (pSoundObject->GetType() == FXType::FX_CONST)
		{
			for (int i = 0; i < 5; i++)
			{
				if (m_pChannel.at(i)->GetIsConst() == FXType::FX_CONST)
				{
					if (m_pChannel.at(i)->GetState() == SoundChannel::SoundChannelState_Free)
					{
						m_pChannel.at(i)->PlaySound(pSoundObject);
						return 0;
					}
					if (isPriority)
					{
						m_pChannel.at(i)->StopSound();
						m_pChannel.at(i)->PlaySound(pSoundObject);
						return 0;
					}
				}
			}
			return 0;
		}
	}
}
	void SoundPlayer::StopSound(int channelIndex)
	{
		if (m_MyType == PT_MUSIC)
		{
			m_pChannel[0]->StopSound();
		}
		else if (m_MyType == PT_MISC)
		{
			for (int i = 0; i < m_pChannel.size(); i++)
			{
				m_pChannel.at(i)->StopSound();
			}
		}
	}

	SoundChannel * SoundPlayer::GetSoundChannel(int anIndex)
	{
		return m_pChannel.at(anIndex);
	}
