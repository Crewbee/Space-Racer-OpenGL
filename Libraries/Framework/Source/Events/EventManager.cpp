#include "FrameworkPCH.h"
#include "EventManager.h"

EventManager::EventManager()
{
	m_Deactivate = false;
}

EventManager::~EventManager()
{
    while( m_Events.empty() == false )
    {
        delete m_Events.front();
        m_Events.pop();
    }
}

void EventManager::QueueEvent(Event* pEvent)
{
    assert( pEvent != 0 );
	if (!m_Deactivate)
	{
		m_Events.push(pEvent);
	}
}

void EventManager::DispatchEvents(GameCore* pGameCore)
{
	if (!m_Deactivate)
	{
		while (m_Events.empty() == false)
		{
			Event* pEvent = m_Events.front();

			pGameCore->OnEvent(pEvent);
			m_Events.pop();

			delete pEvent;
		}
	}
}

void EventManager::DeactivateEventManager(bool isDeactivated)
{
	m_Deactivate = isDeactivated;
	while (m_Events.empty() == false)
	{
		delete m_Events.front();
		m_Events.pop();
	}
}
