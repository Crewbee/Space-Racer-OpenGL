#pragma once
#include "Events/GameEventTypes.h"
#include "../Framework/Source/Events/Event.h"
#include "Helpers/SceneManager.h"

class GameStateChangeEvent : public Event
{
protected:
		
	Game_State m_pNewState;

public:
	GameStateChangeEvent(Game_State aState)
	{
		m_pNewState = aState;
	}
	virtual EventTypes GetEventType() override { return (EventTypes)GameEventType_GameStateChange; }

	Game_State GetState() { return m_pNewState; }
};
