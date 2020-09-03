//
// Copyright (c) 2017-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __SoundPlayer_H__
#define __SoundPlayer_H__

#include <xaudio2.h>
#include "XAudio2/SoundObject.h"

class SoundChannel;

enum PlayerType {
	PT_MUSIC = 0,
	PT_MISC = 1
};

using namespace std;

class SoundPlayer
{
protected:
    static const int MAX_CHANNELS = 5;
	PlayerType m_MyType;
    vector<SoundChannel*> m_pChannel;

public:
	SoundPlayer(PlayerType aType, SoundChannel** aSomeChannels);
    ~SoundPlayer();	

    SoundObject* LoadSound(const char* fullpath, FXType aType);

    int PlaySound(SoundObject* pSoundObject, bool isPriority);
    void StopSound(int channelIndex);

	SoundChannel* GetSoundChannel(int anIndex);
};

#endif //__SoundPlayerXAudio_H__
