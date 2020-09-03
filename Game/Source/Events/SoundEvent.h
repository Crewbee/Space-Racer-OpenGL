#pragma once
#include "Events/GameEventTypes.h"
#include "../Framework/Source/Events/Event.h"

enum Sounds {
	Accelarate = 1,
	Deccelarate,
	Idle,
	Boost,
	TopSpeed,
	Announcer_1,
	Announcer_2,
	Announcer_3,
	Announcer_Go,
	Announcer_Failure,
	Announcer_Success,
	Lasers_1,
	Lasers_2,
	Lasers_3,
	Lasers_4,
	Lasers_5,
	Explosion_1,
	Explosion_2,
	Explosion_3,
	Explosion_4,
	MouseOver,
	ClickButton,
	PlayButton,
	Num_Sounds
};
enum PlayPriority {
	CanPlay = 0,
	MustPlay = 1
};

class SoundEvent : public Event
{
protected:

	Sounds m_MySound;
	PlayPriority m_Priority;

public:
	SoundEvent(Sounds aSound, PlayPriority aPriority)
	{
		m_MySound = aSound;
		m_Priority = aPriority;
	}
	virtual EventTypes GetEventType() override { return (EventTypes)GameEventType_Sound; }

	Sounds GetSound() { return m_MySound; }
	PlayPriority GetPriority() { return m_Priority; }

private:
};