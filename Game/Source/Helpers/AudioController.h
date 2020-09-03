#pragma once
#include <xaudio2.h>
#include "Events/SoundEvent.h"
#include "XAudio2/SoundPlayer.h"

class SoundObject;
class SoundChannel;
class ResourceManager;

enum MusicClipOrder {
	BackGround1 = 1,
	BackGround2 = 2,
	BackGround3 = 3,
	BackGround4 = 4,
	BackGround5 = 5,
	BackGround6 = 6,
	BackGround7 = 7,
	BackGround8 = 8,
	BackGround9 = 9,
	UIBackGround = 10
};

using namespace std;
class AudioController
{
protected:
	const unsigned int MAX_CHANNELS = 5;

	ResourceManager* m_pResources;

	IXAudio2* m_pEngine;
	IXAudio2MasteringVoice* m_pMasteringVoice;

	SoundPlayer* m_MusicPlayer;
	SoundPlayer* m_FXPlayer;

	int m_MusicSampleCount;
	int m_MusicLoopCount[5] = { 5, 6, 7, 8, 9};
	int m_Musicincrement;

	int m_ExplosionIndex[4] = { 0, 1, 2, 3 };
	int m_ExplosionIncrement;
	bool m_PlayBackGroundMusic;
	
	vector<SoundChannel*> m_pMusicChannels;
	vector<SoundChannel*> m_pFXChannels;

	vector<Sounds>m_pExplosions;

	map<Sounds, SoundObject*>m_pSoundFX;
	map<MusicClipOrder, SoundObject*> m_pMusic;

public:
	AudioController(ResourceManager* pResources);
	~AudioController();

	void OnEvent(Event* pEvent);

	void InitSoundPlayers();

	void PlayBackGroundMusic();

	void PlayUIBackgroundMusic();
	void StopUIBackgroundMusic();
	void StopMusic();
	void StopSounds();

	void PlaySoundEffect(Sounds aSound, bool aPriority);

	bool GetPlayBackGroundMusic() { return m_PlayBackGroundMusic; }
	void SetPlayBackGroundMusic(bool playMusic) { m_PlayBackGroundMusic = playMusic; }

	void AddSoundFX(SoundObject* aFXSound, Sounds aName);
	void AddMusicClip(MusicClipOrder aClipHandle, SoundObject* aClip);
	SoundObject* GetMusicClip(MusicClipOrder aClip);

	void SetChannelVolume(float aLevel, PlayerType aPlayer);

	SoundObject* LaserSoundRandom();
	SoundObject* ExplosionSoundRandom();

	SoundPlayer* GetSoundPlayer(PlayerType aSoundPlayerHandle);

	void Reset();

private:

};