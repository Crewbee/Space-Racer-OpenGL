#pragma once
#include "Events/Event.h"

class AudioShutDownComplete : public Event
{

protected:

	EventTypes m_pMyType;

public:

	AudioShutDownComplete()
	{
		m_pMyType = EventTypes::EventType_ShutdownAudio;
	}
	~AudioShutDownComplete()
	{

	}
};
