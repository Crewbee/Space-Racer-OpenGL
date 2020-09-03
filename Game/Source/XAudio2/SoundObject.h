//
// Copyright (c) 2017-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __SoundObject_H__
#define __SoundObject_H__

#include <xaudio2.h>
#include "WaveLoader.h"

enum FXType{
	FX_CONST = 1,
	FX_TEMP,
	FX_MUSIC
};

class SoundObject
{
protected:
    const char* m_pFileBuffer;
    MyWaveDescriptor m_WaveDesc; // Contains pointer to data buffer loaded from file
    XAUDIO2_BUFFER m_XAudioBuffer;
	FXType m_pType;

public:
    SoundObject(FXType aType);
    ~SoundObject();

    void Init(const char* pFileBuffer, MyWaveDescriptor waveDescriptor);

    XAUDIO2_BUFFER* GetXAudioBuffer() { return &m_XAudioBuffer; }
    bool IsValid() { return m_WaveDesc.valid; }

	FXType GetType() { return m_pType; }
};

#endif //__SoundObject_H__
