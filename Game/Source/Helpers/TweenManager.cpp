#include "GamePCH.h"
#include "Helpers/TweenFuncs.h"
#include "TweenManager.h"
#include "Helpers/TweenObject.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/GameCamera.h"

#include "Game/Game.h"

TweenManager::TweenManager(Game* pGame)
{
	m_pGame = pGame;
	m_pOrthoPos.SetIdentity();
	m_pDoesOrtho = false;
	m_pWindowSize = vec2(pGame->GetFramework()->GetWindowWidth(), pGame->GetFramework()->GetWindowWidth());
}
TweenManager::~TweenManager()
{
	for (auto pObject : m_pTranslationTweens)
	{
		for (int i = 0; i < pObject.second.size(); i++)
		{
			delete pObject.second.at(i);
		}
	}
	m_pTranslationTweens.clear();

	for (auto pObject : m_pScalarTweens)
	{
		for (int i = 0; i < pObject.second.size(); i++)
		{
			delete pObject.second.at(i);
		}
	}
	m_pScalarTweens.clear();
}

void TweenManager::Update(float deltaTime)
{
	for (auto pObject : m_pTranslationTweens)
	{
		if (pObject.first->GetIsActive() == true)
		{
			for (int i = 0; i < pObject.second.size(); i++)
			{
				pObject.second.at(i)->Update(deltaTime);
			}
		}
	}
	for (auto pObject : m_pScalarTweens)
	{
		if (pObject.first->GetIsActive() == true)
		{
			for (int i = 0; i < pObject.second.size(); i++)
			{
				pObject.second.at(i)->Update(deltaTime);
			}
		}
	}
}

void TweenManager::Setup1DTween(GameObject* aGameObject, TweenFunc aTween, float* aModifier, float startvalue, float endvalue, double timepassed, double totaltime)
{
	if (m_pTranslationTweens.find(aGameObject) == m_pTranslationTweens.end())
	{
		vector<TweenObject*>aNewTween;
		aNewTween.push_back(new TweenObject(aModifier));
		aNewTween.back()->SetupTween(aTween, startvalue, endvalue, timepassed, totaltime);

		m_pTranslationTweens[aGameObject] = aNewTween;
	}
	else
	{
		m_pTranslationTweens[aGameObject].at(0)->SetupTween(aTween, startvalue, endvalue, timepassed, totaltime);
	}
}
void TweenManager::Setup2DTween(GameObject* aGameObject, TweenFunc aTween, vec2 startvalue, vec2 endvalue, double timepassed, double totaltime)
{
	vec4 perspectiveStart = m_pOrthoPos.GetInverse() * vec4(startvalue.x, startvalue.y, 0.0f, 0.0f);
	vec4 perspectiveEnd = m_pOrthoPos.GetInverse() * vec4(endvalue.x, endvalue.y, 0.0f, 0.0f);

	if (m_pTranslationTweens.find(aGameObject) == m_pTranslationTweens.end())
	{
		vector<TweenObject*>aNewTween;
		if (m_pDoesOrtho)
		{
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->x));
			aNewTween.back()->SetupTween(aTween, perspectiveStart.x, perspectiveEnd.x, timepassed, totaltime);
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->y));
			aNewTween.back()->SetupTween(aTween, perspectiveStart.y, perspectiveEnd.y, timepassed, totaltime);
		}
		else
		{
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->x));
			aNewTween.back()->SetupTween(aTween, startvalue.x, endvalue.x, timepassed, totaltime);
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->y));
			aNewTween.back()->SetupTween(aTween, startvalue.y, endvalue.y, timepassed, totaltime);
		}


		m_pTranslationTweens[aGameObject] = aNewTween;
	}
	else
	{
		if (m_pDoesOrtho)
		{
			m_pTranslationTweens[aGameObject].at(0)->SetupTween(aTween, perspectiveStart.x, perspectiveEnd.x, timepassed, totaltime);
			m_pTranslationTweens[aGameObject].at(1)->SetupTween(aTween, perspectiveStart.y, perspectiveEnd.y, timepassed, totaltime);
		}
		else
		{
			m_pTranslationTweens[aGameObject].at(0)->SetupTween(aTween, startvalue.x, endvalue.x, timepassed, totaltime);
			m_pTranslationTweens[aGameObject].at(1)->SetupTween(aTween, startvalue.y, endvalue.y, timepassed, totaltime);
		}
	}
}
void TweenManager::Setup3DTween(GameObject* aGameObject, TweenFunc aTween, vec3 startvalue, vec3 endvalue, double timepassed, double totaltime)
{
	vec4 perspectiveStart = m_pOrthoPos.GetInverse() * vec4(startvalue.x, startvalue.y, startvalue.z, 0.0f);
	vec4 perspectiveEnd = m_pOrthoPos.GetInverse() * vec4(endvalue.x, endvalue.y, endvalue.z, 0.0f);

	if (m_pTranslationTweens.find(aGameObject) == m_pTranslationTweens.end())
	{
		vector<TweenObject*>aNewTween;

		if (m_pDoesOrtho)
		{
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->x));
			aNewTween.back()->SetupTween(aTween, perspectiveStart.x, perspectiveEnd.x, timepassed, totaltime);
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->y));
			aNewTween.back()->SetupTween(aTween, perspectiveStart.y, perspectiveEnd.y, timepassed, totaltime);
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->z));
			aNewTween.back()->SetupTween(aTween, perspectiveStart.z, perspectiveEnd.z, timepassed, totaltime);
		}
		else
		{
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->x));
			aNewTween.back()->SetupTween(aTween, startvalue.x, endvalue.x, timepassed, totaltime);
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->y));
			aNewTween.back()->SetupTween(aTween, startvalue.y, endvalue.y, timepassed, totaltime);
			aNewTween.push_back(new TweenObject(&aGameObject->GetTweenXYZCoords()->z));
			aNewTween.back()->SetupTween(aTween, startvalue.z, endvalue.z, timepassed, totaltime);
		}


		m_pTranslationTweens[aGameObject] = aNewTween;
	}
	else
	{
		if (m_pDoesOrtho)
		{
			m_pTranslationTweens[aGameObject].at(0)->SetupTween(aTween, perspectiveStart.x, perspectiveEnd.x, timepassed, totaltime);
			m_pTranslationTweens[aGameObject].at(1)->SetupTween(aTween, perspectiveStart.y, perspectiveEnd.y, timepassed, totaltime);
			m_pTranslationTweens[aGameObject].at(2)->SetupTween(aTween, perspectiveStart.z, perspectiveEnd.z, timepassed, totaltime);
		}
		else
		{
			m_pTranslationTweens[aGameObject].at(0)->SetupTween(aTween, startvalue.x, endvalue.x, timepassed, totaltime);
			m_pTranslationTweens[aGameObject].at(1)->SetupTween(aTween, startvalue.y, endvalue.y, timepassed, totaltime);
			m_pTranslationTweens[aGameObject].at(2)->SetupTween(aTween, startvalue.z, endvalue.z, timepassed, totaltime);
		}
	}
}

void TweenManager::Setup2DScalerTween(GameObject* aGameObject, TweenFunc aTween, vec2 startvalue, vec2 endvalue, double timepassed, double totaltime)
{
	if (m_pScalarTweens.find(aGameObject) == m_pScalarTweens.end())
	{
		vector<TweenObject*>aNewTween;

		aNewTween.push_back(new TweenObject(&aGameObject->GetTweenScaleCoords()->x));
		aNewTween.back()->SetupTween(aTween, startvalue.x, endvalue.x, timepassed, totaltime);
		aNewTween.push_back(new TweenObject(&aGameObject->GetTweenScaleCoords()->y));
		aNewTween.back()->SetupTween(aTween, startvalue.y, endvalue.y, timepassed, totaltime);

		m_pScalarTweens[aGameObject] = aNewTween;
	}
	else
	{

		m_pScalarTweens[aGameObject].at(0)->SetupTween(aTween, startvalue.x, endvalue.x, timepassed, totaltime);
		m_pScalarTweens[aGameObject].at(1)->SetupTween(aTween, startvalue.y, endvalue.y, timepassed, totaltime);

	}
}

void TweenManager::OnSurfaceChange(GameObject* anObject, const mat4 OrthoInverse, float* aStartingPos, float* aEndingPos)
{
	vec2 NewWindowSize = vec2(m_pGame->GetFramework()->GetWindowWidth(), m_pGame->GetFramework()->GetWindowHeight());
	if (m_pTranslationTweens.find(anObject) != m_pTranslationTweens.end())
	{
		if (m_pDoesOrtho)
		{
			m_pOrthoPos = OrthoInverse;
			if (m_pTranslationTweens[anObject].size() > 1)
			{
				if (m_pTranslationTweens[anObject].size() == 2)
				{
					vec4 NewPos = m_pOrthoPos.GetInverse() * vec4(aStartingPos[0], aStartingPos[1], 0.0f, 0.0f);
					vec4 NewEnd = m_pOrthoPos.GetInverse() * vec4(aEndingPos[0], aEndingPos[1], 0.0f, 0.0f);


					m_pTranslationTweens[anObject].at(0)->SetOrthoValue(vec2(NewPos.x, NewEnd.x));
					m_pTranslationTweens[anObject].at(1)->SetOrthoValue(vec2(NewPos.y, NewEnd.y));
				}
				else if (m_pTranslationTweens[anObject].size() == 3)
				{
					vec4 NewPos = m_pOrthoPos.GetInverse() * vec4(aStartingPos[0], aStartingPos[1], aStartingPos[2], 0.0f);
					vec4 NewEnd = m_pOrthoPos.GetInverse() * vec4(aEndingPos[0], aEndingPos[1], aEndingPos[2], 0.0f);


					m_pTranslationTweens[anObject].at(0)->SetOrthoValue(vec2(NewPos.x, NewEnd.x));
					m_pTranslationTweens[anObject].at(1)->SetOrthoValue(vec2(NewPos.y, NewEnd.y));
					m_pTranslationTweens[anObject].at(2)->SetOrthoValue(vec2(NewPos.z, NewEnd.z));
				}
			}
		}
	}


	m_pWindowSize = NewWindowSize;
}

TweenState TweenManager::GetTweenStateByObject(GameObject * anObject)
{
	if (m_pTranslationTweens.find(anObject) != m_pTranslationTweens.end())
	{
		int NumTweens = m_pTranslationTweens.find(anObject)->second.size();
		TweenState totalState = TWEEN_RUNNING;

		for (int i = 0; i < NumTweens; i++)
		{
			if (m_pTranslationTweens.find(anObject)->second.at(i)->GetMyState() != TWEEN_SUCCESS)
			{
				totalState = m_pTranslationTweens.find(anObject)->second.at(i)->GetMyState();
				return totalState;
			}
			else
			{
				totalState = TWEEN_SUCCESS;
			}
		}

		return totalState;
	}
	return TWEEN_NULL;
}

void TweenManager::SetTweenStateByObject(GameObject * anObject, TweenState aState)
{
	if (m_pTranslationTweens.find(anObject) != m_pTranslationTweens.end())
	{
		for (int i = 0; i < m_pTranslationTweens[anObject].size(); i++)
		{
			m_pTranslationTweens[anObject].at(i)->SetTweenState(aState);
		}
	}
	if (m_pScalarTweens.find(anObject) != m_pScalarTweens.end())
	{
		for (int i = 0; i < m_pScalarTweens[anObject].size(); i++)
		{
			m_pScalarTweens[anObject].at(i)->SetTweenState(aState);
		}
	}
}

bool TweenManager::GetDoesObjectTween(GameObject * anObject)
{
	if (m_pTranslationTweens.find(anObject) != m_pTranslationTweens.end())
	{
		return true;
	}
	else if (m_pScalarTweens.find(anObject) != m_pScalarTweens.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TweenManager::RemoveTweensFromObject(GameObject* anObject)
{
	if (m_pTranslationTweens.find(anObject) != m_pTranslationTweens.end())
	{
		for (int i = 0; i < m_pTranslationTweens.find(anObject)->second.size(); i++)
		{
			delete m_pTranslationTweens.find(anObject)->second.at(i);
		}

		m_pTranslationTweens.erase(anObject);
	}
	if (m_pScalarTweens.find(anObject) != m_pScalarTweens.end())
	{
		for (int i = 0; i < m_pScalarTweens.find(anObject)->second.size(); i++)
		{
			delete m_pScalarTweens.find(anObject)->second.at(i);
		}

		m_pScalarTweens.erase(anObject);
	}
}

void TweenManager::ResetTweensOnObject(GameObject * anObject)
{
	if (m_pTranslationTweens.find(anObject) != m_pTranslationTweens.end())
	{
		for (int i = 0; i < m_pTranslationTweens[anObject].size(); i++)
		{
			m_pTranslationTweens[anObject].at(i)->ResetTween();
		}
	}
	if (m_pScalarTweens.find(anObject) != m_pScalarTweens.end())
	{
		for (int i = 0; i < m_pScalarTweens[anObject].size(); i++)
		{
			m_pScalarTweens[anObject].at(i)->ResetTween();
		}
	}
}
