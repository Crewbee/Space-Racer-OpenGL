#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "HUDScene.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/ScoreDisplay.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"

#include "Game/Game.h"

HUDScene::HUDScene(Game * pGame, ResourceManager * pResources, AudioController * pAudioController, SceneManager * aSceneManager, TweenManager* pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{
	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();
	m_pCountDownTimer = 6.0f;
	m_pCountDown = Three;
}

HUDScene::~HUDScene()
{
	for (auto pObject : m_pHUDObjects)
	{
		delete pObject;
	}
}

void HUDScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	UIObject* aScoreTitle = new UIObject(this, "ScoreDisplay", vec2(-0.88f, 0.8f), vec2(0.22f, 0.18f), m_pResources->GetMesh("UIDigitFrame"), m_pResources->GetMaterial("ScoreTitleMaterial"));
	vec4 aScoreTitlePos = m_pOrthoPos.GetInverse() * vec4(aScoreTitle->GetOffset().x, aScoreTitle->GetOffset().y, 0.0, 0.0);
	aScoreTitle->SetPosition(vec2(aScoreTitlePos.x, aScoreTitlePos.y));
	aScoreTitle->SetScale(vec3(aScoreTitle->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), aScoreTitle->GetBaseScale().y, 0.0f));
	AddUIObject(aScoreTitle);


	ScoreDisplay* aScoreDisplay = new ScoreDisplay(this, "ScoreDisplay", vec2(-0.75f, 0.8f), vec2(0.05f, 0.1f), m_pResources->GetMesh("UIDigitFrame"), m_pResources->GetMaterial("0"), m_pResources, m_pOrthoPos);
	vec4 aScoreDisplayPos = m_pOrthoPos.GetInverse() * vec4(aScoreDisplay->GetOffset().x, aScoreDisplay->GetOffset().y, 0.0, 0.0);
	aScoreDisplay->SetPosition(vec2(aScoreDisplayPos.x, aScoreDisplayPos.y));
	aScoreDisplay->SetScale(vec3(aScoreDisplay->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), aScoreDisplay->GetBaseScale().y, 0.0f));
	AddUIObject(aScoreDisplay);

	UIObject* aCountDown3 = new UIObject(this, "CountDown3", vec2(0.0f, 0.0f), vec2(0.5f, 1.0f), m_pResources->GetMesh("RenderMesh"), m_pResources->GetMaterial("3"));
	vec4 aCountDown3Pos = m_pOrthoPos.GetInverse() * vec4(aCountDown3->GetOffset().x, aCountDown3->GetOffset().y, 0.0, 0.0);
	aCountDown3->SetPosition(vec2(aCountDown3Pos.x, aCountDown3Pos.y));
	aCountDown3->SetScale(vec3(aCountDown3->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), aCountDown3->GetBaseScale().y, 0.0f));
	aCountDown3->SetAlphaChannel(0.0f);
	aCountDown3->SetDoesFade(150.0f, false);
	aCountDown3->SetPlaysSound(true);
	AddUIObject(aCountDown3);

	UIObject* aCountDown2 = new UIObject(this, "CountDown2", vec2(0.0f, 0.0f), vec2(0.5f, 1.0f), m_pResources->GetMesh("RenderMesh"), m_pResources->GetMaterial("2"));
	vec4 aCountDown2Pos = m_pOrthoPos.GetInverse() * vec4(aCountDown2->GetOffset().x, aCountDown2->GetOffset().y, 0.0, 0.0);
	aCountDown2->SetPosition(vec2(aCountDown2Pos.x, aCountDown2Pos.y));
	aCountDown2->SetScale(vec3(aCountDown2->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), aCountDown2->GetBaseScale().y, 0.0f));
	aCountDown2->SetAlphaChannel(0.0f);
	aCountDown2->SetPlaysSound(true);
	AddUIObject(aCountDown2);

	UIObject* aCountDown1 = new UIObject(this, "CountDown1", vec2(0.0f, 0.0f), vec2(0.5f, 1.0f), m_pResources->GetMesh("RenderMesh"), m_pResources->GetMaterial("1"));
	vec4 aCountDown1Pos = m_pOrthoPos.GetInverse() * vec4(aCountDown1->GetOffset().x, aCountDown1->GetOffset().y, 0.0, 0.0);
	aCountDown1->SetPosition(vec2(aCountDown1Pos.x, aCountDown1Pos.y));
	aCountDown1->SetScale(vec3(aCountDown1->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), aCountDown1->GetBaseScale().y, 0.0f));
	aCountDown1->SetAlphaChannel(0.0f);
	aCountDown1->SetPlaysSound(true);
	AddUIObject(aCountDown1);

	UIObject* aCountDownGo = new UIObject(this, "CountDownGo", vec2(0.0f, 0.0f), vec2(1.2f, 1.0f), m_pResources->GetMesh("RenderMesh"), m_pResources->GetMaterial("Go"));
	vec4 aCountDownGoPos = m_pOrthoPos.GetInverse() * vec4(aCountDownGo->GetOffset().x, aCountDownGo->GetOffset().y, 0.0, 0.0);
	aCountDownGo->SetPosition(vec2(aCountDownGoPos.x, aCountDownGoPos.y));
	aCountDownGo->SetScale(vec3(aCountDownGo->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), aCountDownGo->GetBaseScale().y, 0.0f));
	aCountDownGo->SetAlphaChannel(0.0f);
	aCountDownGo->SetPlaysSound(true);
	AddUIObject(aCountDownGo);
}

void HUDScene::OnEvent(Event * pEvent)
{
	for (auto pObject : m_pHUDObjects)
	{
		if (pObject != nullptr)
		{
			pObject->OnEvent(pEvent);
		}
	}
}

void HUDScene::Update(float deltaTime)
{
	for (auto pObject : m_pHUDObjects)
	{
		pObject->Update(deltaTime);
	}
	if (m_pSceneManager->GetGameState() == Game_State::GS_BEGINPLAY)
	{
		switch (m_pCountDown) {
		case CountDown::Three:

			if (GetUIObjectByName("CountDown3")->GetAlphaChannel() > 0.7f && GetUIObjectByName("CountDown3")->GetPlaysSound() == true)
			{
				Event* psound = new SoundEvent(Sounds::Announcer_3, MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);

				GetUIObjectByName("CountDown3")->SetPlaysSound(false);
			}
			if (GetUIObjectByName("CountDown3")->GetIsFading() == false)
			{
				GetUIObjectByName("CountDown2")->SetDoesFade(150.0f, false);
				m_pCountDown = Two;
			}
			break;

		case CountDown::Two:
			if (GetUIObjectByName("CountDown2")->GetAlphaChannel() > 0.7f && GetUIObjectByName("CountDown2")->GetPlaysSound() == true)
			{
				Event* psound = new SoundEvent(Sounds::Announcer_2, MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);

				GetUIObjectByName("CountDown2")->SetPlaysSound(false);
			}
			if (GetUIObjectByName("CountDown2")->GetIsFading() == false)
			{
				GetUIObjectByName("CountDown1")->SetDoesFade(150.0f, false);
				m_pCountDown = One;
			}
			break;

		case CountDown::One:
			if (GetUIObjectByName("CountDown1")->GetAlphaChannel() > 0.7f && GetUIObjectByName("CountDown1")->GetPlaysSound() == true)
			{
				Event* psound = new SoundEvent(Sounds::Announcer_1, MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);

				GetUIObjectByName("CountDown1")->SetPlaysSound(false);
			}
			if (GetUIObjectByName("CountDown1")->GetIsFading() == false)
			{
				GetUIObjectByName("CountDownGo")->SetDoesFade(150.0f, false);
				m_pCountDown = GO;
			}
			break;
		case CountDown::GO:
			m_pSceneManager->SetGameState(Game_State::GS_RUNNING);
			break;
		}

	}
	if (GetUIObjectByName("CountDownGo")->GetAlphaChannel() > 0.7f && GetUIObjectByName("CountDownGo")->GetPlaysSound() == true)
	{
		Event* psound = new SoundEvent(Sounds::Announcer_Go, MustPlay);
		m_pGame->GetEventManager()->QueueEvent(psound);

		GetUIObjectByName("CountDownGo")->SetPlaysSound(false);
	}

}

void HUDScene::Draw()
{
	for (auto pObject : m_pHUDObjects)
	{
		pObject->Draw(m_pGameCamera);
	}
}

UIObject * HUDScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pHUDObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}
	return nullptr;
}

void HUDScene::OnScenePush()
{
	for (auto pObject : m_pHUDObjects)
	{
		pObject->SetIsActive(true);
	}
}

void HUDScene::OnScenePop()
{
	for (auto pObject : m_pHUDObjects)
	{
		pObject->SetIsActive(false);
	}
}

void HUDScene::Reset()
{
	//GetUIObjectByName("ScoreDisplay")->SetScore(0);
	//GetUIObjectByName("CountDown3")->SetDoesFade(150, false);

	//for (auto pUIObject : m_pHUDObjects)
	//{
	//	pUIObject->SetPlaysSound(true);
	//}

	//m_pCountDown = CountDown::Three;

	for (auto pUIObject : m_pHUDObjects)
	{
		delete pUIObject;
	}

	m_pHUDObjects.clear();

	delete m_pGameCamera;

	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();
	m_pCountDownTimer = 6.0f;
	m_pCountDown = Three;

	LoadContent();
}
