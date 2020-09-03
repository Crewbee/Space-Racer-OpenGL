#pragma once
#include "Events/GameEventTypes.h"
#include "../Framework/Source/Events/Event.h"

class AddScoreEvent : public Event
{
protected:

	int m_Amount;

public:
	AddScoreEvent(int aAmount)
	{
		m_Amount = aAmount;
	}
	virtual EventTypes GetEventType() override { return (EventTypes)GameEventType_Score; }

	int GetScore() { return m_Amount; }

private:
};