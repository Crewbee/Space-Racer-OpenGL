#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "MenuScene.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"
#include "Helpers/TweenFuncs.h"

#include "Game/Game.h"

MenuScene::MenuScene(Game * pGame, ResourceManager * pResources, AudioController * pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{
	m_pWindowSize.Set(0, 0);
	m_pOrthoPos.SetIdentity();
	m_pStartingMenuPos = vec2(1.25f, 0.6f);
	m_pMenuPos = vec2(-0.87f, 1.1f);
}

MenuScene::~MenuScene()
{
	for (auto pObject : m_pMenuObjects)
	{
		delete pObject;
	}
}

void MenuScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	UIObject* Background = new UIObject(this, "BackGround", m_pStartingMenuPos, vec2(2.6f, 4.0f), m_pResources->GetMesh("UIMesh"), m_pResources->GetMaterial("UIMaterial"));
	vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0, 0.0);
	Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));
	AddUIObject(Background);

	UIObject* PlayButton = new UIObject(this, "PlayButton", vec2(0.0f, 0.2f), vec2(0.05f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("PlayButtonMaterial"));
	vec4 PlayButtonPos = m_pOrthoPos.GetInverse() * vec4(PlayButton->GetOffset().x, PlayButton->GetOffset().y, 0.0, 0.0);
	PlayButton->SetPosition(vec2(PlayButtonPos.x, PlayButtonPos.y));
	PlayButton->SetScale(vec3(PlayButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), PlayButton->GetBaseScale().y, 0.0f));
	PlayButton->SetPlaysSound(true);
	AddUIObject(PlayButton);

	UIObject* SelectPlay = new UIObject(this, "SelectPlay", vec2(0.0f, 0.2f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectPlayMaterial"));
	vec4 SelectPlayPos = m_pOrthoPos.GetInverse() * vec4(SelectPlay->GetOffset().x, SelectPlay->GetOffset().y, 0.0, 0.0);
	SelectPlay->SetPosition(vec2(SelectPlayPos.x, SelectPlayPos.y));
	SelectPlay->SetScale(vec3(SelectPlay->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectPlay->GetBaseScale().y, 0.0f));
	SelectPlay->SetAlphaChannel(0.0f);
	AddUIObject(SelectPlay);

	UIObject* OptionsButton = new UIObject(this, "OptionsButton", vec2(0.0f, -0.15f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("OptionsButtonMaterial"));
	vec4 OptionsButtonPos = m_pOrthoPos.GetInverse() * vec4(OptionsButton->GetOffset().x, OptionsButton->GetOffset().y, 0.0, 0.0);
	OptionsButton->SetPosition(vec2(OptionsButtonPos.x, OptionsButtonPos.y));
	OptionsButton->SetScale(vec3(OptionsButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), OptionsButton->GetBaseScale().y, 0.0f));
	OptionsButton->SetPlaysSound(true);
	AddUIObject(OptionsButton);

	UIObject* SelectOption = new UIObject(this, "SelectOption", vec2(0.0f, -0.15f), vec2(0.07f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectOptionMaterial"));
	vec4 SelectOptionPos = m_pOrthoPos.GetInverse() * vec4(SelectOption->GetOffset().x, SelectOption->GetOffset().y, 0.0, 0.0);
	SelectOption->SetPosition(vec2(SelectOptionPos.x, SelectOptionPos.y));
	SelectOption->SetScale(vec3(SelectOption->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectOption->GetBaseScale().y, 0.0f));
	SelectOption->SetAlphaChannel(0.0f);
	AddUIObject(SelectOption);

	UIObject* ExitButton = new UIObject(this, "ExitButton", vec2(0.0f, -0.5f), vec2(0.05f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("ExitButtonMaterial"));
	vec4 ExitButtonPos = m_pOrthoPos.GetInverse() * vec4(ExitButton->GetOffset().x, ExitButton->GetOffset().y, 0.0, 0.0);
	ExitButton->SetPosition(vec2(ExitButtonPos.x, ExitButtonPos.y));
	ExitButton->SetScale(vec3(ExitButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), ExitButton->GetBaseScale().y, 0.0f));
	ExitButton->SetPlaysSound(true);
	AddUIObject(ExitButton);

	UIObject* SelectExit = new UIObject(this, "SelectExit", vec2(0.0f, -0.5f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectExitMaterial"));
	vec4 SelectExitPos = m_pOrthoPos.GetInverse() * vec4(SelectExit->GetOffset().x, SelectExit->GetOffset().y, 0.0, 0.0);
	SelectExit->SetPosition(vec2(SelectExitPos.x, SelectExitPos.y));
	SelectExit->SetScale(vec3(SelectExit->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectExit->GetBaseScale().y, 0.0f));
	SelectExit->SetAlphaChannel(0.0f);
	AddUIObject(SelectExit);

	m_pTweenManager->SetInternalOrthoMatrix(m_pOrthoPos);
	m_pTweenManager->SetInternalOrthoPerspectiveCalculations(true);

	m_pTweenManager->Setup2DTween(Background, TweenFunc_SineEaseInOut, m_pStartingMenuPos, m_pMenuPos, 0.0, 2.0);
	Background->SetIsTweening(true);

}

void MenuScene::OnEvent(Event * pEvent)
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

		vec2 ButtonPos = vec2(GetUIObjectByName("PlayButton")->GetPosition().x, GetUIObjectByName("PlayButton")->GetPosition().y);
		vec2 ButtonSize = vec2(GetUIObjectByName("PlayButton")->GetMeshSize().x, GetUIObjectByName("PlayButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("PlayButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectPlay")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				SoundEvent* psound = new SoundEvent(Sounds::PlayButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);
				m_pAudio->StopUIBackgroundMusic();
				m_pSceneManager->SetGameState(Game_State::GS_BEGINPLAY);
				m_pSceneManager->PopScene();
			}
		}
		else
		{
			GetUIObjectByName("SelectPlay")->SetAlphaChannel(0.0f);
			GetUIObjectByName("PlayButton")->SetAlphaChannel(1.0f);
		}

		ButtonPos = vec2(GetUIObjectByName("OptionsButton")->GetPosition().x, GetUIObjectByName("OptionsButton")->GetPosition().y);
		ButtonSize = vec2(GetUIObjectByName("OptionsButton")->GetMeshSize().x, GetUIObjectByName("OptionsButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("OptionsButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectOption")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				UIObject* background = GetUIObjectByName("BackGround");
				SoundEvent* psound = new SoundEvent(Sounds::ClickButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);

				if (m_pStartingMenuPos != vec2(-0.45f, -1.0f))
				{
					m_pStartingMenuPos.Set(-0.45f, -1.0f);
					background->SetPositionOffset(m_pStartingMenuPos);
					vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(m_pStartingMenuPos.x, m_pStartingMenuPos.y, 0.0, 0.0);
					background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));

					m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));
					m_pTweenManager->Setup2DTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, m_pStartingMenuPos, m_pMenuPos, 0.0, 2.0);
				}
				else
				{
					m_pTweenManager->ResetTweensOnObject(GetUIObjectByName("BackGround"));
				}

				m_pSceneManager->PushScene(ST_OPTIONS);
			}
		}
		else
		{
			GetUIObjectByName("SelectOption")->SetAlphaChannel(0.0f);
			GetUIObjectByName("OptionsButton")->SetAlphaChannel(1.0f);
		}

		ButtonPos = vec2(GetUIObjectByName("ExitButton")->GetPosition().x, GetUIObjectByName("ExitButton")->GetPosition().y);
		ButtonSize = vec2(GetUIObjectByName("ExitButton")->GetMeshSize().x, GetUIObjectByName("ExitButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("ExitButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectExit")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputStates::InputState_Pressed && pInput->GetID() == 0)
			{
                m_pGame->GetFramework()->SetCloseRequest(true);
			}
		}
		else
		{
			GetUIObjectByName("SelectExit")->SetAlphaChannel(0.0f);
			GetUIObjectByName("ExitButton")->SetAlphaChannel(1.0f);
		}

	}
	for (auto pObject : m_pMenuObjects)
	{
		if (pObject != nullptr)
		{
			pObject->OnEvent(pEvent);
		}
	}
}

void MenuScene::Update(float deltaTime)
{
	for (auto pObject : m_pMenuObjects)
	{
		pObject->Update(deltaTime);
	}
	m_pTweenManager->Update(deltaTime);
	if (m_pTweenManager->GetTweenStateByObject(GetUIObjectByName("BackGround")) != TWEEN_SUCCESS)
	{
		for (auto pObject : m_pMenuObjects)
		{
			pObject->SetIsOpaque(false);
		}
	}
	else
	{
		vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(m_pMenuPos.x, m_pMenuPos.y, 0.0f, 0.0f);
		GetUIObjectByName("BackGround")->SetPositionOffset(m_pMenuPos);
		GetUIObjectByName("BackGround")->SetPosition(vec2(RelativePos.x, RelativePos.y));

		for (auto pObject : m_pMenuObjects)
		{
			pObject->SetIsOpaque(true);
		}
	}

	m_pAudio->PlayUIBackgroundMusic();
}

void MenuScene::Draw()
{
	for (auto pObject : m_pMenuObjects)
	{
		if (pObject->GetName() == "BackGround")
		{
			pObject->Draw(m_pGameCamera);
		}
		else if (pObject->GetIsOpaque())
		{
			pObject->Draw(m_pGameCamera);
		}
	}
}
void MenuScene::OnSurfaceChange(float percentChange)
{
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	for (auto pObject : m_pMenuObjects)
	{
		vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(pObject->GetOffset().x, pObject->GetOffset().y, 0.0f, 0.0f);
		pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f));
		pObject->SetPosition(vec2(RelativePos.x, RelativePos.y));
		m_pTweenManager->OnSurfaceChange(pObject, m_pOrthoPos, &m_pStartingMenuPos.x, &m_pMenuPos.x);
	}
}
UIObject * MenuScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pMenuObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}
	return nullptr;
}

void MenuScene::OnScenePush()
{
	for (auto pObject : m_pMenuObjects)
	{
		pObject->SetIsActive(true);
	}
}

void MenuScene::OnScenePop()
{
	for (auto pObject : m_pMenuObjects)
	{
		pObject->SetIsActive(false);
	}
}

void MenuScene::Reset()
{
	//m_pStartingMenuPos = vec2(1.25f, 0.6f);
	//m_pMenuPos = vec2(-0.87f, 1.1f);

	//m_pWindowSize = m_pGameCamera->GetWindowSize();
	//m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	//UIObject* Background = GetUIObjectByName("BackGround");
	//Background->SetPosition(m_pStartingMenuPos);
	//Background->SetPositionOffset(vec2(2.6f, 4.0f));
	//vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0, 0.0);
	//Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	//Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));
    //m_pTweenManager->Setup2DTween(Background, TweenFunc_SineEaseInOut, m_pStartingMenuPos, m_pMenuPos, 0.0, 2.0);
	m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));

	for (auto pUIObject : m_pMenuObjects)
	{
		delete pUIObject;
	}

	m_pMenuObjects.clear();

	delete m_pGameCamera;

	m_pWindowSize.Set(0, 0);
	m_pOrthoPos.SetIdentity();
	m_pStartingMenuPos = vec2(1.25f, 0.6f);
	m_pMenuPos = vec2(-0.87f, 1.1f);

	LoadContent();

}
