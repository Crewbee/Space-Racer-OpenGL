#pragma once
#include"Helpers/TweenFuncs.h"
#include"Helpers/TweenManager.h"

class TweenManager;

class TweenObject
{
protected:

    TweenFunc m_pMyTweenFunc;
    float m_pStartValue;
    float m_pEndValue;
    double m_pTimeElapsed;
    double m_pMaxTime;

    float* m_pModifier;

	TweenState m_pMyState;

public:
    TweenObject(float* aModifiedValue);
    ~TweenObject();

    void Update(float deltaTime);

    void SetupTween(TweenFunc aTween, float startvalue, float endValue, double timepassed, double totaltime);

	void SetOrthoValue(vec2 aValue) { m_pStartValue = aValue.x; m_pEndValue = aValue.y; }
	vec2 GetOrthoValue() { return vec2(m_pStartValue, m_pEndValue); }

	TweenState GetMyState() { return m_pMyState; }
	void SetTweenState(TweenState aState) { m_pMyState = aState; }

	void ResetTween();

	void ResetScale(float aPercentage);
private:

};
