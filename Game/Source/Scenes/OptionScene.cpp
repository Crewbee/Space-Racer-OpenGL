#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "OptionScene.h"
#include "Scenes/LightingScene.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"
#include "GameObjects/Player.h"

#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/ResourceManager.h"
#include "Helpers/TweenManager.h"

#include "Game/Game.h"

OptionScene::OptionScene(Game * pGame, ResourceManager * pResources, AudioController * pAudio, SceneManager * aSceneManager, TweenManager* pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudio, pTweenManager)
{
	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();
	m_pOptionsPos.Set(-0.45f, -1.0f);
	m_pStartingOptionPos.Set(-0.87f, 1.1f);

}

OptionScene::~OptionScene()
{
	for (auto pObject : m_pOptionObjects)
	{
		delete pObject;
	}
}

void OptionScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	UIObject* Background = new UIObject(this, "BackGround", m_pStartingOptionPos, vec2(2.6f, 4.0f), m_pResources->GetMesh("UIMesh"), m_pResources->GetMaterial("UIMaterial"));
	vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0, 0.0);
	Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));
	AddUIObject(Background);

	UIObject* AudioButton = new UIObject(this, "AudioButton", vec2(0.0f, 0.3f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("AudioButtonMaterial"));
	vec4 AudioButtonPos = m_pOrthoPos.GetInverse() * vec4(AudioButton->GetOffset().x, AudioButton->GetOffset().y, 0.0, 0.0);
	AudioButton->SetPosition(vec2(AudioButtonPos.x, AudioButtonPos.y));
	AudioButton->SetScale(vec3(AudioButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), AudioButton->GetBaseScale().y, 0.0f));
	AudioButton->SetPlaysSound(true);
	AddUIObject(AudioButton);

	UIObject* SelectAudio = new UIObject(this, "SelectAudio", vec2(0.0f, 0.3f), vec2(0.07f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectAudioMaterial"));
	vec4 SelectAudioPos = m_pOrthoPos.GetInverse() * vec4(SelectAudio->GetOffset().x, SelectAudio->GetOffset().y, 0.0, 0.0);
	SelectAudio->SetPosition(vec2(SelectAudioPos.x, SelectAudioPos.y));
	SelectAudio->SetScale(vec3(SelectAudio->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectAudio->GetBaseScale().y, 0.0f));
	SelectAudio->SetAlphaChannel(0.0f);
	AddUIObject(SelectAudio);

	UIObject* LudicrousSpeedButton = new UIObject(this, "LudicrousSpeedButton", vec2(0.0f, 0.0f), vec2(0.1f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("LudicrousSpeedMaterial"));
	vec4 LudicrousSpeedButtonPos = m_pOrthoPos.GetInverse() * vec4(LudicrousSpeedButton->GetOffset().x, LudicrousSpeedButton->GetOffset().y, 0.0, 0.0);
	LudicrousSpeedButton->SetPosition(vec2(LudicrousSpeedButtonPos.x, LudicrousSpeedButtonPos.y));
	LudicrousSpeedButton->SetScale(vec3(LudicrousSpeedButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), LudicrousSpeedButton->GetBaseScale().y, 0.0f));
	LudicrousSpeedButton->SetPlaysSound(true);
	AddUIObject(LudicrousSpeedButton);

	UIObject* SelectLudicrousSpeed = new UIObject(this, "SelectLudicrousSpeed", vec2(0.0f, 0.0f), vec2(0.11f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectLudicrousSpeedMaterial"));
	vec4 SelectLudicrousSpeedPos = m_pOrthoPos.GetInverse() * vec4(SelectLudicrousSpeed->GetOffset().x, SelectLudicrousSpeed->GetOffset().y, 0.0, 0.0);
	SelectLudicrousSpeed->SetPosition(vec2(SelectLudicrousSpeedPos.x, SelectLudicrousSpeedPos.y));
	SelectLudicrousSpeed->SetScale(vec3(SelectLudicrousSpeed->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectLudicrousSpeed->GetBaseScale().y, 0.0f));
	SelectLudicrousSpeed->SetAlphaChannel(0.0f);
	AddUIObject(SelectLudicrousSpeed);

	UIObject* MainMenuButton = new UIObject(this, "MainMenuButton", vec2(0.0f, -0.3f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("MainMenuMaterial"));
	vec4 MainMenuButtonPos = m_pOrthoPos.GetInverse() * vec4(MainMenuButton->GetOffset().x, MainMenuButton->GetOffset().y, 0.0, 0.0);
	MainMenuButton->SetPosition(vec2(MainMenuButtonPos.x, MainMenuButtonPos.y));
	MainMenuButton->SetScale(vec3(MainMenuButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), MainMenuButton->GetBaseScale().y, 0.0f));
	MainMenuButton->SetPlaysSound(true);
	AddUIObject(MainMenuButton);

	UIObject* SelectMainMenu = new UIObject(this, "SelectMainMenu", vec2(0.0f, -0.3f), vec2(0.07f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectMainMenuMaterial"));
	vec4 SelectExitPos = m_pOrthoPos.GetInverse() * vec4(SelectMainMenu->GetOffset().x, SelectMainMenu->GetOffset().y, 0.0, 0.0);
	SelectMainMenu->SetPosition(vec2(SelectExitPos.x, SelectExitPos.y));
	SelectMainMenu->SetScale(vec3(SelectMainMenu->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectMainMenu->GetBaseScale().y, 0.0f));
	SelectMainMenu->SetAlphaChannel(0.0f);
	AddUIObject(SelectMainMenu);


	m_pTweenManager->SetInternalOrthoMatrix(m_pOrthoPos);
	m_pTweenManager->SetInternalOrthoPerspectiveCalculations(true);

	m_pTweenManager->Setup2DTween(Background, TweenFunc_SineEaseInOut, m_pStartingOptionPos, m_pOptionsPos, 0.0, 2.0);
	Background->SetIsTweening(true);
}

void OptionScene::OnEvent(Event * pEvent)
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

		vec2 ButtonPos = vec2(GetUIObjectByName("AudioButton")->GetPosition().x, GetUIObjectByName("AudioButton")->GetPosition().y);
		vec2 ButtonSize = vec2(GetUIObjectByName("AudioButton")->GetMeshSize().x, GetUIObjectByName("AudioButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("AudioButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectAudio")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				UIObject* background = GetUIObjectByName("BackGround");
				SoundEvent* psound = new SoundEvent(Sounds::ClickButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);

				if (m_pStartingOptionPos != vec2(0.975f, -1.5f))
				{
					m_pStartingOptionPos.Set(0.975f, -1.30f);
					background->SetPositionOffset(m_pStartingOptionPos);
					vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(m_pStartingOptionPos.x, m_pStartingOptionPos.y, 0.0, 0.0);
					background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));

					vec2 ScalarBegin = vec2(background->GetTweenScaleCoords()->x, background->GetTweenScaleCoords()->y) * 1.4f;
					vec2 ScalarEnd = vec2(background->GetTweenScaleCoords()->x, background->GetTweenScaleCoords()->y);

					m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));

					m_pTweenManager->Setup2DScalerTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, ScalarBegin, ScalarEnd, 0.0, 2.0);
					m_pTweenManager->Setup2DTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, m_pStartingOptionPos, m_pOptionsPos, 0.0, 2.0);

					background->SetScale(vec3(ScalarBegin.x, ScalarBegin.y, 0.0f));
				}
				else
				{
					m_pTweenManager->ResetTweensOnObject(GetUIObjectByName("BackGround"));
				}
				m_pSceneManager->PushScene(ST_AUDIO);
			}
		}
		else
		{
			GetUIObjectByName("SelectAudio")->SetAlphaChannel(0.0f);
			GetUIObjectByName("AudioButton")->SetAlphaChannel(1.0f);
		}

		ButtonPos = vec2(GetUIObjectByName("LudicrousSpeedButton")->GetPosition().x, GetUIObjectByName("LudicrousSpeedButton")->GetPosition().y);
		ButtonSize = vec2(GetUIObjectByName("LudicrousSpeedButton")->GetMeshSize().x, GetUIObjectByName("LudicrousSpeedButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("LudicrousSpeedButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectLudicrousSpeed")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				SoundEvent* psound = new SoundEvent(Sounds::ClickButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);
				bool HoldButtonAlpha = GetUIObjectByName("LudicrousSpeedButton")->GetHoldAlpha();
				bool HoldSelectAlpha = GetUIObjectByName("SelectLudicrousSpeed")->GetHoldAlpha();
				GetUIObjectByName("LudicrousSpeedButton")->SetHoldAlpha(!HoldButtonAlpha);
				GetUIObjectByName("SelectLudicrousSpeed")->SetHoldAlpha(!HoldSelectAlpha);

				bool UseLudicrous = m_pSceneManager->GetScene(ST_GAMEPLAY)->GetPlayer()->GetUseLudicrousSpeed();
				m_pSceneManager->GetScene(ST_GAMEPLAY)->GetPlayer()->SetUseLudicrousSpeed(!UseLudicrous);

			}
		}
		else
		{
			GetUIObjectByName("SelectLudicrousSpeed")->SetAlphaChannel(0.0f);
			GetUIObjectByName("LudicrousSpeedButton")->SetAlphaChannel(1.0f);
		}

		ButtonPos = vec2(GetUIObjectByName("MainMenuButton")->GetPosition().x, GetUIObjectByName("MainMenuButton")->GetPosition().y);
		ButtonSize = vec2(GetUIObjectByName("MainMenuButton")->GetMeshSize().x, GetUIObjectByName("MainMenuButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("MainMenuButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectMainMenu")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				UIObject* background = GetUIObjectByName("BackGround");
				SoundEvent* psound = new SoundEvent(Sounds::ClickButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);
				if (m_pStartingOptionPos != vec2(-0.87f, 1.1f))
				{
					m_pStartingOptionPos.Set(-0.87f, 1.1f);
					background->SetPositionOffset(m_pStartingOptionPos);
					vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(m_pStartingOptionPos.x, m_pStartingOptionPos.y, 0.0, 0.0);
					background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));

					m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));
					m_pTweenManager->Setup2DTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, m_pStartingOptionPos, m_pOptionsPos, 0.0, 2.0);
				}
				else
				{
					m_pTweenManager->ResetTweensOnObject(GetUIObjectByName("BackGround"));
				}
				m_pSceneManager->PopScene();
			}
		}
		else
		{
			GetUIObjectByName("SelectMainMenu")->SetAlphaChannel(0.0f);
			GetUIObjectByName("MainMenuButton")->SetAlphaChannel(1.0f);
		}

	}
	for (auto pObject : m_pOptionObjects)
	{
		if (pObject != nullptr)
		{
			pObject->OnEvent(pEvent);
		}
	}
}

void OptionScene::Update(float deltaTime)
{
	for (auto pObject : m_pOptionObjects)
	{
		pObject->Update(deltaTime);
	}
	m_pTweenManager->Update(deltaTime);
	if(m_pTweenManager->GetTweenStateByObject(GetUIObjectByName("BackGround")) != TWEEN_SUCCESS)
	{
		for (auto pObject : m_pOptionObjects)
		{
			pObject->SetIsOpaque(false);
		}
	}
	else
	{
		vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(m_pOptionsPos.x, m_pOptionsPos.y, 0.0f, 0.0f);
		GetUIObjectByName("BackGround")->SetPositionOffset(m_pOptionsPos);
		GetUIObjectByName("BackGround")->SetPosition(vec2(RelativePos.x, RelativePos.y));
		for (auto pObject : m_pOptionObjects)
		{
			pObject->SetIsOpaque(true);
		}
	}

	m_pAudio->PlayUIBackgroundMusic();
}

void OptionScene::Draw()
{
	for (auto pObject : m_pOptionObjects)
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

void OptionScene::OnSurfaceChange(float percentChange)
{
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	for (auto pObject : m_pOptionObjects)
	{
		vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(pObject->GetOffset().x, pObject->GetOffset().y, 0.0f, 0.0f);
		pObject->SetPosition(vec2(RelativePos.x, RelativePos.y));
		m_pTweenManager->OnSurfaceChange(pObject, m_pOrthoPos, &m_pStartingOptionPos.x, &m_pOptionsPos.x);
		if (pObject->GetIsTweening())
		{
			if (m_pSceneManager->GetCurrentSceneType() == ST_AUDIO)
			{
				pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f) * 1.4f);
			}
			else
			{
				pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f));
			}
		}
		else
		{
			pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f));
		}

	}
}

UIObject * OptionScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pOptionObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}
	return nullptr;
}

void OptionScene::OnScenePush()
{
	for (auto pObject : m_pOptionObjects)
	{
		pObject->SetIsActive(true);
	}
}

void OptionScene::OnScenePop()
{
	for (auto pObject : m_pOptionObjects)
	{
		pObject->SetIsActive(false);
	}
}

void OptionScene::Reset()
{
	//m_pOptionsPos.Set(-0.45f, -1.0f);
	//m_pStartingOptionPos.Set(-0.87f, 1.1f);

	//m_pWindowSize = m_pGameCamera->GetWindowSize();
	//m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	//UIObject* Background = GetUIObjectByName("BackGround");
	//Background->SetPosition(m_pStartingOptionPos);
	//Background->SetPositionOffset(vec2(2.6f, 4.0f));
	//vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0, 0.0);
	//Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	//Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));


	//m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));

 //   m_pTweenManager->Setup2DTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, m_pStartingOptionPos, m_pOptionsPos, 0.0, 2.0);

	m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));

	for (auto pUIObject : m_pOptionObjects)
	{
		delete pUIObject;
	}

	m_pOptionObjects.clear();

	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();
	m_pOptionsPos.Set(-0.45f, -1.0f);
	m_pStartingOptionPos.Set(-0.87f, 1.1f);

	delete m_pGameCamera;

	LoadContent();
}
