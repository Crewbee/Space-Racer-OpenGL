#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "GameOverScene.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"

#include "Game/Game.h"

GameOverScene::GameOverScene(Game* pGame, ResourceManager * pResources, AudioController * pAudioController, SceneManager * aSceneManager, TweenManager * pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{
	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();
	m_pAudio = pAudioController;
}

GameOverScene::~GameOverScene()
{
	for (auto pObject : m_pGameOverObjects)
	{
		delete pObject;
	}
}

void GameOverScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	UIObject* GameOverTitle = new UIObject(this, "GameOverTitle", vec2(0.0f, 0.2f), vec2(0.2f, 0.26f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("GameOverMaterial"));
	vec4 GameOverTitlePos = m_pOrthoPos.GetInverse() * vec4(GameOverTitle->GetOffset().x, GameOverTitle->GetOffset().y, 0.0f, 0.0f);
	GameOverTitle->SetPosition(vec2(GameOverTitlePos.x, GameOverTitlePos.y));
	GameOverTitle->SetScale(vec3(GameOverTitle->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), GameOverTitle->GetBaseScale().y, 0.0f));
	AddUIObject(GameOverTitle);

	UIObject* WinTitle = new UIObject(this, "WinTitle", vec2(0.0f, 0.2f), vec2(0.2f, 0.26f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("WinMaterial"));
	vec4 WinTitlePos = m_pOrthoPos.GetInverse() * vec4(WinTitle->GetOffset().x, WinTitle->GetOffset().y, 0.0f, 0.0f);
	WinTitle->SetPosition(vec2(WinTitlePos.x, WinTitlePos.y));
	WinTitle->SetScale(vec3(WinTitle->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), WinTitle->GetBaseScale().y, 0.0f));
	WinTitle->SetAlphaChannel(0.0f);
	AddUIObject(WinTitle);

	UIObject* MainMenuButton = new UIObject(this, "MainMenuButton", vec2(0.0f, -0.4f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("MainMenuMaterial"));
	vec4 MainMenuButtonPos = m_pOrthoPos.GetInverse() * vec4(MainMenuButton->GetOffset().x, MainMenuButton->GetOffset().y, 0.0, 0.0);
	MainMenuButton->SetPosition(vec2(MainMenuButtonPos.x, MainMenuButtonPos.y));
	MainMenuButton->SetScale(vec3(MainMenuButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), MainMenuButton->GetBaseScale().y, 0.0f));
	MainMenuButton->SetIsOpaque(true);
	MainMenuButton->SetPlaysSound(true);
	AddUIObject(MainMenuButton);

	UIObject* SelectMainMenu = new UIObject(this, "SelectMainMenu", vec2(0.0f, -0.4f), vec2(0.07f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectMainMenuMaterial"));
	vec4 SelectExitPos = m_pOrthoPos.GetInverse() * vec4(SelectMainMenu->GetOffset().x, SelectMainMenu->GetOffset().y, 0.0, 0.0);
	SelectMainMenu->SetPosition(vec2(SelectExitPos.x, SelectExitPos.y));
	SelectMainMenu->SetScale(vec3(SelectMainMenu->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectMainMenu->GetBaseScale().y, 0.0f));
	MainMenuButton->SetIsOpaque(true);
	SelectMainMenu->SetAlphaChannel(0.0f);
	AddUIObject(SelectMainMenu);
}

void GameOverScene::OnEvent(Event * pEvent)
{
	InputEvent* pInput = static_cast<InputEvent*>(pEvent);
	if (pInput->GetInputDeviceType() == InputDeviceType_Mouse)
	{
		vec2 MousePos = pInput->GetPosition();
		MousePos = MousePos / m_pWindowSize;
		MousePos = MousePos * 2.0f - 1.0f;
		MousePos.y *= -1.0f;

		vec4 ConvertToClip = m_pOrthoPos.GetInverse() * vec4(MousePos.x, MousePos.y, 0.0f, 0.0f);

		MousePos = vec2(ConvertToClip.x, ConvertToClip.y);

		vec2 ButtonPos = vec2(GetUIObjectByName("MainMenuButton")->GetPosition().x, GetUIObjectByName("MainMenuButton")->GetPosition().y);
		vec2 ButtonSize = vec2(GetUIObjectByName("MainMenuButton")->GetMeshSize().x, GetUIObjectByName("MainMenuButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("MainMenuButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectMainMenu")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				SoundEvent* psound = new SoundEvent(Sounds::ClickButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);

				m_pSceneManager->ResetGame();
			
			}
		}
		else
		{
			GetUIObjectByName("SelectMainMenu")->SetAlphaChannel(0.0f);
			GetUIObjectByName("MainMenuButton")->SetAlphaChannel(1.0f);
		}
	}

}

void GameOverScene::Update(float deltaTime)
{
}

void GameOverScene::Draw()
{
	for (auto pObject : m_pGameOverObjects)
	{
		pObject->Draw(m_pGameCamera);
	}
}

UIObject * GameOverScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pGameOverObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}
	return nullptr;
}

void GameOverScene::OnScenePush()
{
	for (auto pObject : m_pGameOverObjects)
	{
		pObject->SetIsActive(true);
	}

	if (m_pSceneManager->GetGameState() == Game_State::GS_WINGAME)
	{
		GetUIObjectByName("GameOverTitle")->SetAlphaChannel(0.0f);
		GetUIObjectByName("WinTitle")->SetAlphaChannel(1.0f);
	}
}

void GameOverScene::OnScenePop()
{
	for (auto pObject : m_pGameOverObjects)
	{
		pObject->SetIsActive(false);
	}
}
void GameOverScene::Reset()
{
    //GetUIObjectByName("GameOverTitle")->SetAlphaChannel(1.0f);
    //GetUIObjectByName("WinTitle")->SetAlphaChannel(0.0f);

	for (auto pUIObject : m_pGameOverObjects)
	{
		delete pUIObject;
	}

	m_pGameOverObjects.clear();

	delete m_pGameCamera;

	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();

	LoadContent();
}

