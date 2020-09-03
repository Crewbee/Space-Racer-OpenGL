#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "PauseScene.h"

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

PauseScene::PauseScene(Game * pGame, ResourceManager * pResources, AudioController * pAudioController, SceneManager * aSceneManager, TweenManager * pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{

}

PauseScene::~PauseScene()
{
	for (auto pObject : m_pPauseObjects)
	{
		delete pObject;
	}
}

void PauseScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	UIObject* PauseTitle = new UIObject(this, "PauseTitle", vec2(0.0f, 0.2f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("PauseTitleMaterial"));
	vec4 PauseTitlePos = m_pOrthoPos.GetInverse() * vec4(PauseTitle->GetOffset().x, PauseTitle->GetOffset().y, 0.0f, 0.0f);
	PauseTitle->SetPosition(vec2(PauseTitlePos.x, PauseTitlePos.y));
	PauseTitle->SetScale(vec3(PauseTitle->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), PauseTitle->GetBaseScale().y, 0.0f));
	PauseTitle->SetPlaysSound(true);
	AddUIObject(PauseTitle);


}

void PauseScene::OnEvent(Event * pEvent)
{
}

void PauseScene::Update(float deltaTime)
{
}

void PauseScene::Draw()
{
	for (auto pObject : m_pPauseObjects)
	{
		pObject->Draw(m_pGameCamera);
	}
}

UIObject * PauseScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pPauseObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}
	return nullptr;
}

void PauseScene::OnScenePush()
{
	for (auto pObject : m_pPauseObjects)
	{
		pObject->SetIsActive(true);
	}
}

void PauseScene::OnScenePop()
{
	for (auto pObject : m_pPauseObjects)
	{
		pObject->SetIsActive(false);
	}
}

void PauseScene::Reset()
{

}
