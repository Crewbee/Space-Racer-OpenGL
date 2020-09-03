#pragma once
#include "Events/Event.h"

class ShutdownAudioEvent : public Event
{

protected:

	EventTypes m_pMyType;

public:

	ShutdownAudioEvent()
	{

		m_pMyType = EventType_ShutdownAudio;

	}
	~ShutdownAudioEvent() {}

	EventTypes GetType() { return m_pMyType; }

};