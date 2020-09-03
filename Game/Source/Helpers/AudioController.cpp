#include "GamePCH.h"
#include <xaudio2.h>

#include "AudioController.h"
#include "XAudio2/SoundObject.h"
#include "XAudio2/SoundChannel.h"
#include "XAudio2/VoiceCallback.h"
#include "Helpers/ResourceManager.h"

#include "Events/GameEventTypes.h"

AudioController::AudioController(ResourceManager* pResources)
{
	m_pResources = pResources;
	m_MusicSampleCount = 1;
	m_Musicincrement = 0;
	m_PlayBackGroundMusic = true;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT result = XAudio2Create(&m_pEngine);

	if (result == S_OK)
	{
		result = m_pEngine->CreateMasteringVoice(&m_pMasteringVoice);
	}

	if (result != S_OK)
	{
		OutputMessage("Error initializing XAudio\n");
		exit(1);
	}

	for (unsigned int i = 0; i < MAX_CHANNELS; i++)
	{
		SoundChannel* aMusicChannel = new SoundChannel(m_pEngine);
		SoundChannel* aFXChannel = new SoundChannel(m_pEngine);
		m_pMusicChannels.push_back(aMusicChannel);
		m_pFXChannels.push_back(aFXChannel);
	}

	m_pFXChannels.at(0)->SetIsConst(FXType::FX_CONST);

	m_pExplosions.push_back(Sounds::Explosion_1);
	m_pExplosions.push_back(Sounds::Explosion_2);
	m_pExplosions.push_back(Sounds::Explosion_3);
	m_pExplosions.push_back(Sounds::Explosion_4);

	m_ExplosionIncrement = 0;
}

AudioController::~AudioController()
{
	for (unsigned int i = 0; i < MAX_CHANNELS; i++)
	{
		delete m_pMusicChannels.at(i);
		delete m_pFXChannels.at(i);
	}
	for (auto pSound : m_pSoundFX)
	{
		delete pSound.second;
	}
	for (auto pMusic : m_pMusic)
	{
		delete pMusic.second;
	}
	delete m_MusicPlayer;
	delete m_FXPlayer;

	m_pEngine->Release();

	CoUninitialize();
}

void AudioController::InitSoundPlayers()
{
	m_MusicPlayer = new SoundPlayer(PlayerType::PT_MUSIC, &m_pMusicChannels[0]);
	m_FXPlayer = new SoundPlayer(PlayerType::PT_MISC, &m_pFXChannels[0]);
}

void AudioController::OnEvent(Event* pEvent)
{
	if (pEvent)
	{
		SoundEvent* soundEvent = static_cast<SoundEvent*>(pEvent);
		if (soundEvent->GetEventType() == GameEventTypes::GameEventType_Sound)
		{
			Sounds aSound = soundEvent->GetSound();
			bool aPriority = soundEvent->GetPriority();
			PlaySoundEffect(aSound, aPriority);
		}
	}
}

void AudioController::PlayBackGroundMusic()
{
	if (m_PlayBackGroundMusic)
	{
		if (m_MusicPlayer->GetSoundChannel(0)->GetState() == SoundChannel::SoundChannelState_Free)
		{
			if (m_MusicSampleCount < 10)
			{
				m_MusicPlayer->PlaySound(GetMusicClip(MusicClipOrder(m_MusicSampleCount)), false);

				m_MusicSampleCount++;
			}
			else
			{
				m_Musicincrement = m_Musicincrement % 5;

				m_MusicPlayer->PlaySound(GetMusicClip(MusicClipOrder(m_MusicLoopCount[m_Musicincrement])), false);

				m_Musicincrement++;
			}
		}
	}
}

void AudioController::PlayUIBackgroundMusic()
{
	if (m_MusicPlayer->GetSoundChannel(0)->GetState() == SoundChannel::SoundChannelState_Free)
	{
		m_MusicPlayer->PlaySound(GetMusicClip(UIBackGround), false);
	}
}

void AudioController::StopUIBackgroundMusic()
{
	if (m_MusicPlayer->GetSoundChannel(0)->GetState() == SoundChannel::SoundChannelState_Playing)
	{
		m_MusicPlayer->StopSound(0);
	}
}

void AudioController::StopMusic()
{
	if (m_MusicPlayer->GetSoundChannel(0)->GetState() == SoundChannel::SoundChannelState_Playing)
	{
		m_MusicPlayer->StopSound(0);
	}
}

void AudioController::StopSounds()
{
	for (int i = 0; i < m_pFXChannels.size(); i++)
	{
		if (m_FXPlayer->GetSoundChannel(i)->GetState() == SoundChannel::SoundChannelState_Playing)
		{
			m_FXPlayer->StopSound(i);
		}
	}
}

void AudioController::PlaySoundEffect(Sounds aSound, bool isPriority)
{
	if (aSound == Lasers_1)
	{
		SoundObject* laser = LaserSoundRandom();
		m_FXPlayer->PlaySound(laser, isPriority);
	}
	else if (aSound == Explosion_1)
	{
		SoundObject* explosion = ExplosionSoundRandom();
		m_FXPlayer->PlaySound(explosion, isPriority);
	}
	else
	{
		m_FXPlayer->PlaySound(m_pSoundFX.find(aSound)->second, isPriority);
	}
}

void AudioController::AddSoundFX(SoundObject* aFXSound, Sounds aName)
{
	assert(m_pSoundFX.find(aName) == m_pSoundFX.end());

	m_pSoundFX[aName] = aFXSound;
}

void AudioController::AddMusicClip(MusicClipOrder aClipHandle, SoundObject* aClip)
{
	assert(m_pMusic.find(aClipHandle) == m_pMusic.end());

	m_pMusic[aClipHandle] = aClip;

}

SoundObject* AudioController::GetMusicClip(MusicClipOrder aClip)
{
	auto it = m_pMusic.find(aClip);
	if (it == m_pMusic.end())
		return nullptr;

	return it->second;

}

void AudioController::SetChannelVolume(float aLevel, PlayerType aPlayer)
{
	if (aPlayer == PT_MUSIC)
	{
		for (unsigned int i = 0; i < m_pMusicChannels.size(); i++)
		{
			m_pMusicChannels.at(i)->GetSourceVoice()->SetVolume(aLevel);
		}
	}
	else if (aPlayer == PT_MISC)
	{
		for (unsigned int i = 0; i < m_pFXChannels.size(); i++)
		{
			m_pFXChannels.at(i)->GetSourceVoice()->SetVolume(aLevel);
		}
	}
}

SoundObject* AudioController::LaserSoundRandom()
{
	int randomLaserIndex = rand() % 300;

	if (randomLaserIndex < 50)
	{
		return m_pSoundFX[Lasers_1];
	}
	else if (randomLaserIndex < 120)
	{
		return m_pSoundFX[Lasers_2];
	}
	else if (randomLaserIndex < 230)
	{
		return m_pSoundFX[Lasers_3];
	}
	else if (randomLaserIndex < 270)
	{
		return m_pSoundFX[Lasers_4];
	}
	else
	{
		return m_pSoundFX[Lasers_5];
	}
}

SoundObject * AudioController::ExplosionSoundRandom()
{
	if (m_ExplosionIncrement < 3)
	{
		Sounds anExplosion = m_pExplosions.at(m_ExplosionIndex[m_ExplosionIncrement]);

		return m_pSoundFX.find(anExplosion)->second;
		m_ExplosionIncrement++;
	}
	else
	{
		vector<int>newShuffle;
		m_ExplosionIncrement = 0;


		while (newShuffle.size() < 4)
		{
			int index = rand() % 4;

			if (newShuffle.size() == 0)
			{
				newShuffle.push_back(index);
			}
			else
			{
				bool doesExist = false;
				for (int i = 0; i < newShuffle.size(); i++)
				{
					if (newShuffle.at(i) == index)
					{
						doesExist = true;
					}
				}
				if (doesExist = false)
				{
					newShuffle.push_back(index);
				}

				if (newShuffle.size() == 3)
				{
					int lastNum = 8;
					for (int j = 0; j < newShuffle.size(); j++)
					{
						lastNum -= newShuffle.at(j);
					}

					newShuffle.push_back(lastNum);
				}
			}
		}

		for (int i = 0; i < newShuffle.size(); i++)
		{
			m_ExplosionIndex[i] = newShuffle.at(i);
		}

		Sounds anExplosion = m_pExplosions.at(m_ExplosionIndex[m_ExplosionIncrement]);

		return m_pSoundFX.find(anExplosion)->second;
		m_ExplosionIncrement++;
	}
}

SoundPlayer* AudioController::GetSoundPlayer(PlayerType aSoundPlayerHandle)
{
	if (aSoundPlayerHandle == PT_MUSIC)
	{
		return m_MusicPlayer;
	}
	else if (aSoundPlayerHandle == PT_MISC)
	{
		return m_FXPlayer;
	}
	return nullptr;
}

void AudioController::Reset()
{
	for (auto pMusic : m_pMusicChannels)
	{
		pMusic->StopSound();
	}
	for (auto pFX : m_pFXChannels)
	{
		pFX->StopSound();
	}
	m_MusicSampleCount = 0;
	m_MusicSampleCount = 1;
}
