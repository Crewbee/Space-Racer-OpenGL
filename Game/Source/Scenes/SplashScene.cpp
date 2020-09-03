#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "SplashScene.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"
#include "GameObjects/UIObject.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"

SplashScene::SplashScene(Game * pGame, ResourceManager * pResources, AudioController * pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{
	m_pWindowSize.Set(0, 0);
	m_pOrthoPos.SetIdentity();
}

SplashScene::~SplashScene()
{
	for (auto pObject : m_pSplashScreenObjects)
	{
		delete pObject;
	}
}

void SplashScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();


	UIObject* Background = new UIObject(this, "BackGround", vec2(1.25f, 0.6f), vec2(2.6f, 4.0f), m_pResources->GetMesh("UIMesh"), m_pResources->GetMaterial("UIMaterial"));
	vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0, 0.0);
	Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));
	AddUIObject(Background);

	UIObject* EnterIcon = new UIObject(this, "EnterIcon", vec2(0.0f, -0.6f), vec2(0.07f, 0.07f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("PressEnterMaterial"));
	vec4 EnterIconPos = m_pOrthoPos.GetInverse() * vec4(EnterIcon->GetOffset().x, EnterIcon->GetOffset().y, 0.0, 0.0);
	EnterIcon->SetPosition(vec2(EnterIconPos.x, EnterIconPos.y));
	EnterIcon->SetScale(vec3(EnterIcon->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), EnterIcon->GetBaseScale().y, 0.0f));
	EnterIcon->SetDoesFade(160.0f, true);
	AddUIObject(EnterIcon);

}

void SplashScene::OnEvent(Event * pEvent)
{
	for (auto pObject : m_pSplashScreenObjects)
	{
		if (pObject != nullptr)
		{
			pObject->OnEvent(pEvent);
		}
	}
}

void SplashScene::Update(float deltaTime)
{
	if (m_pWindowSize != m_pGameCamera->GetWindowSize())
	{
		m_pOrthoPos = *m_pGameCamera->GetProjMatrix();
		m_pWindowSize = m_pGameCamera->GetWindowSize();
		for (auto pObject : m_pSplashScreenObjects)
		{
			vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(pObject->GetOffset().x, pObject->GetOffset().y, 0.0f, 0.0f);
			pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f));
			pObject->SetPosition(vec2(RelativePos.x, RelativePos.y));
		}
	}
	for (auto pObject : m_pSplashScreenObjects)
	{
		pObject->Update(deltaTime);
	}
	m_pAudio->PlayUIBackgroundMusic();
}

void SplashScene::Draw()
{
	for (auto pObject : m_pSplashScreenObjects)
	{
		pObject->Draw(m_pGameCamera);
	}
}

UIObject * SplashScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pSplashScreenObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}

	return nullptr;
}

void SplashScene::Reset()
{

	for (auto pUIObject : m_pSplashScreenObjects)
	{
		delete pUIObject;
	}

	m_pSplashScreenObjects.clear();

	delete m_pGameCamera;

	m_pWindowSize.Set(0, 0);
	m_pOrthoPos.SetIdentity();

	LoadContent();
}
