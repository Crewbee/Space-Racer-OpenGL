#include "GamePCH.h"

#include "TweenObject.h"
#include "Helpers/TweenFuncs.h"
#include "Helpers/TweenManager.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"

TweenObject::TweenObject(float* aModifiedValue)
{
    m_pMyTweenFunc = nullptr;
    m_pStartValue = 0.0f;
    m_pEndValue = 0.0f;
    m_pTimeElapsed = 0.0;
    m_pMaxTime = 0.0;
    m_pModifier = aModifiedValue;

	m_pMyState = TWEEN_PAUSED;
}
TweenObject::~TweenObject()
{

}
void TweenObject::Update(float deltaTime)
{
	if (m_pMyState == TWEEN_PAUSED || m_pMyState == TWEEN_RUNNING)
	{
		if (m_pTimeElapsed < m_pMaxTime)
		{
			m_pMyState = TWEEN_RUNNING;
			m_pTimeElapsed += deltaTime;
			*m_pModifier = m_pMyTweenFunc(m_pStartValue, m_pEndValue - m_pStartValue, m_pTimeElapsed, m_pMaxTime);
		}
		else
		{
			m_pMyState = TWEEN_SUCCESS;
		}
	}
}
void TweenObject::SetupTween(TweenFunc aTween, float startvalue, float endvalue, double timepassed, double totaltime)
{
    m_pMyTweenFunc = aTween;
    m_pStartValue = startvalue;
	m_pEndValue = endvalue;
    m_pTimeElapsed = timepassed;
    m_pMaxTime = totaltime;

	m_pMyState = TWEEN_PAUSED;
}

void TweenObject::ResetTween()
{
	*m_pModifier = m_pStartValue;
	m_pTimeElapsed = 0.0;

	m_pMyState = TWEEN_PAUSED;
}

void TweenObject::ResetScale(float aPercentage)
{
	m_pStartValue *= aPercentage;
	m_pEndValue *= aPercentage;

	if (m_pMyState != TWEEN_SUCCESS)
	{
		*m_pModifier = m_pStartValue;
	}
	else
	{
		*m_pModifier = m_pEndValue;
	}
}
