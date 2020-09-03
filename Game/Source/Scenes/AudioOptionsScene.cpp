#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "AudioOptionsScene.h"
#include "Scenes/LightingScene.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/UISliderObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"
#include "GameObjects/Player.h"

#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/ResourceManager.h"
#include "Helpers/TweenManager.h"

#include "Game/Game.h"

AudioOptionsScene::AudioOptionsScene(Game * pGame, ResourceManager * pResources, AudioController * pAudio, SceneManager * aSceneManager, TweenManager* pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudio, pTweenManager)
{
	m_pWindowSize.Set(0, 0);
	m_pOrthoPos.SetIdentity();
	m_pAudioOptionsPos.Set(0.975f, -1.5f);
	m_pStartingAudioPos.Set(-0.45f, -1.0f);
	m_pTargetScale.Set(0.0f, 0.0f, 0.0f, 0.0f);
}

AudioOptionsScene::~AudioOptionsScene()
{
	for (auto pObject : m_pAudioOptionsObjects)
	{
		delete pObject;
	}
}

void AudioOptionsScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	UIObject* Background = new UIObject(this, "BackGround", m_pStartingAudioPos, vec2(2.6f, 4.0f), m_pResources->GetMesh("UIMesh"), m_pResources->GetMaterial("UIMaterial"));
	vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0f, 0.0f);
	Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));
	AddUIObject(Background);

	UIObject* AudioTitle = new UIObject(this, "AudioTitle", vec2(0.0f, 0.8f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("AudioTitleMaterial"));
	vec4 AudioTitlePos = m_pOrthoPos.GetInverse() * vec4(AudioTitle->GetOffset().x, AudioTitle->GetOffset().y, 0.0f, 0.0f);
	AudioTitle->SetPosition(vec2(AudioTitlePos.x, AudioTitlePos.y));
	AudioTitle->SetScale(vec3(AudioTitle->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), AudioTitle->GetBaseScale().y, 0.0f));
	AudioTitle->SetPlaysSound(true);
	AddUIObject(AudioTitle);

	UIObject* NoMusicButton = new UIObject(this, "NoMusicButton", vec2(0.0f, 0.5f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("NoMusicMaterial"));
	vec4 NoMusicPos = m_pOrthoPos.GetInverse() * vec4(NoMusicButton->GetOffset().x, NoMusicButton->GetOffset().y, 0.0f, 0.0f);
	NoMusicButton->SetPosition(vec2(NoMusicPos.x, NoMusicPos.y));
	NoMusicButton->SetScale(vec3(NoMusicButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), NoMusicButton->GetBaseScale().y, 0.0f));
	NoMusicButton->SetPlaysSound(true);
	AddUIObject(NoMusicButton);

	UIObject* SelectNoMusic = new UIObject(this, "SelectNoMusic", vec2(0.0f, 0.5f), vec2(0.07f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectNoMusicMaterial"));
	vec4 SelectNoMusicPos = m_pOrthoPos.GetInverse() * vec4(SelectNoMusic->GetOffset().x, SelectNoMusic->GetOffset().y, 0.0f, 0.0f);
	SelectNoMusic->SetPosition(vec2(SelectNoMusicPos.x, SelectNoMusicPos.y));
	SelectNoMusic->SetScale(vec3(SelectNoMusic->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectNoMusic->GetBaseScale().y, 0.0f));
	SelectNoMusic->SetAlphaChannel(0.0f);
	AddUIObject(SelectNoMusic);

	m_pMusicSlider = new UISliderObject(this, "MusicSlider", vec2(0.0f, 0.15f), vec2(0.12f, 0.35f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("MusicSliderMaterial"), m_pResources, m_pOrthoPos, PlayerType::PT_MUSIC);
	vec4 MusicSliderPos = m_pOrthoPos.GetInverse() * vec4(m_pMusicSlider->GetOffset().x, m_pMusicSlider->GetOffset().y, 0.0f, 0.0f);
	m_pMusicSlider->SetPosition(vec2(MusicSliderPos.x, MusicSliderPos.y));
	m_pMusicSlider->SetScale(vec3(m_pMusicSlider->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), m_pMusicSlider->GetBaseScale().y, 0.0f));
	m_pMusicSlider->SetPlaysSound(false);
	AddUIObject(m_pMusicSlider);

	m_pFXSlider = new UISliderObject(this, "FXSlider", vec2(0.0f, -0.3f), vec2(0.12f, 0.35f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("FXSliderMaterial"), m_pResources, m_pOrthoPos, PlayerType::PT_MISC);
	vec4 FXSliderPos = m_pOrthoPos.GetInverse() * vec4(m_pFXSlider->GetOffset().x, m_pFXSlider->GetOffset().y, 0.0f, 0.0f);
	m_pFXSlider->SetPosition(vec2(FXSliderPos.x, FXSliderPos.y));
	m_pFXSlider->SetScale(vec3(m_pFXSlider->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), m_pFXSlider->GetBaseScale().y, 0.0f));
	m_pFXSlider->SetPlaysSound(false);
	AddUIObject(m_pFXSlider);

	UIObject* OptionsButton = new UIObject(this, "OptionsButton", vec2(0.0f, -0.7f), vec2(0.06f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("OptionsButtonMaterial"));
	vec4 OptionsButtonPos = m_pOrthoPos.GetInverse() * vec4(OptionsButton->GetOffset().x, OptionsButton->GetOffset().y, 0.0, 0.0);
	OptionsButton->SetPosition(vec2(OptionsButtonPos.x, OptionsButtonPos.y));
	OptionsButton->SetScale(vec3(OptionsButton->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), OptionsButton->GetBaseScale().y, 0.0f));
	OptionsButton->SetPlaysSound(true);
	AddUIObject(OptionsButton);

	UIObject* SelectOption = new UIObject(this, "SelectOption", vec2(0.0f, -0.7f), vec2(0.07f, 0.15f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SelectOptionMaterial"));
	vec4 SelectOptionPos = m_pOrthoPos.GetInverse() * vec4(SelectOption->GetOffset().x, SelectOption->GetOffset().y, 0.0, 0.0);
	SelectOption->SetPosition(vec2(SelectOptionPos.x, SelectOptionPos.y));
	SelectOption->SetScale(vec3(SelectOption->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), SelectOption->GetBaseScale().y, 0.0f));
	SelectOption->SetAlphaChannel(0.0f);
	AddUIObject(SelectOption);

	m_pTweenManager->SetInternalOrthoMatrix(m_pOrthoPos);
	m_pTweenManager->SetInternalOrthoPerspectiveCalculations(true);

	vec2 ScalarEnd = vec2(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y) * 1.4f;

	m_pTweenManager->Setup2DTween(Background, TweenFunc_SineEaseInOut, m_pStartingAudioPos, m_pAudioOptionsPos, 0.0, 2.0);
	m_pTweenManager->Setup2DScalerTween(Background, TweenFunc_SineEaseInOut, vec2(Background->GetTweenScaleCoords()->x, Background->GetTweenScaleCoords()->y), ScalarEnd, 0.0, 2.0);
	Background->SetIsTweening(true);

}

void AudioOptionsScene::OnEvent(Event * pEvent)
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

		vec2 ButtonPos = vec2(GetUIObjectByName("NoMusicButton")->GetPosition().x, GetUIObjectByName("NoMusicButton")->GetPosition().y);
		vec2 ButtonSize = vec2(GetUIObjectByName("NoMusicButton")->GetMeshSize().x, GetUIObjectByName("NoMusicButton")->GetMeshSize().y);

		if (MousePos.x > (ButtonPos.x - (ButtonSize.x / 2)) && MousePos.x < (ButtonPos.x + (ButtonSize.x / 2)) && MousePos.y >(ButtonPos.y - (ButtonSize.y / 2)) && MousePos.y < (ButtonPos.y + (ButtonSize.y / 2)))
		{
			GetUIObjectByName("NoMusicButton")->SetAlphaChannel(0.0f);
			GetUIObjectByName("SelectNoMusic")->SetAlphaChannel(1.0f);

			if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
			{
				SoundEvent* psound = new SoundEvent(Sounds::ClickButton, PlayPriority::MustPlay);
				m_pGame->GetEventManager()->QueueEvent(psound);
				bool HoldButtonAlpha = GetUIObjectByName("NoMusicButton")->GetHoldAlpha();
				bool HoldSelectAlpha = GetUIObjectByName("SelectNoMusic")->GetHoldAlpha();
				GetUIObjectByName("NoMusicButton")->SetHoldAlpha(!HoldButtonAlpha);
				GetUIObjectByName("SelectNoMusic")->SetHoldAlpha(!HoldSelectAlpha);

				bool PlayMusic = m_pAudio->GetPlayBackGroundMusic();
				m_pAudio->SetPlayBackGroundMusic(!PlayMusic);
			}

		}
		else
		{
			GetUIObjectByName("SelectNoMusic")->SetAlphaChannel(0.0f);
			GetUIObjectByName("NoMusicButton")->SetAlphaChannel(1.0f);
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

				m_pStartingAudioPos.Set(-0.45f, -1.0f);
				background->SetPositionOffset(m_pStartingAudioPos);

				vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(m_pStartingAudioPos.x, m_pStartingAudioPos.y, 0.0, 0.0);
				background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));

				vec3 ScalarBegin = *m_pSceneManager->GetScene(ST_MENU)->GetUIObjectByName("BackGround")->GetTweenScaleCoords();
				vec2 ScalarEnd = vec2(background->GetTweenScaleCoords()->x, background->GetTweenScaleCoords()->y);

				m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));

				m_pTweenManager->Setup2DScalerTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, vec2(ScalarBegin.x, ScalarBegin.y), ScalarEnd, 0.0, 2.0);
				m_pTweenManager->Setup2DTween(GetUIObjectByName("BackGround"), TweenFunc_SineEaseInOut, m_pStartingAudioPos, m_pAudioOptionsPos, 0.0, 2.0);

				m_pSceneManager->PopScene();
			}
		}
		else
		{
			GetUIObjectByName("SelectOption")->SetAlphaChannel(0.0f);
			GetUIObjectByName("OptionsButton")->SetAlphaChannel(1.0f);
		}
	}

	for (auto pObject : m_pAudioOptionsObjects)
	{
		if (pObject != nullptr)
		{
			pObject->OnEvent(pEvent);
		}
	}
}

void AudioOptionsScene::Update(float deltaTime)
{
	m_pTweenManager->Update(deltaTime);

	m_pGameCamera->Update(deltaTime);

	for (auto pObject : m_pAudioOptionsObjects)
	{
		pObject->Update(deltaTime);
	}

	if (m_pTweenManager->GetTweenStateByObject(GetUIObjectByName("BackGround")) != TWEEN_SUCCESS)
	{
		for (auto pObject : m_pAudioOptionsObjects)
		{
			pObject->SetIsOpaque(false);
		}

	}
	else
	{
		vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(m_pAudioOptionsPos.x, m_pAudioOptionsPos.y, 0.0f, 0.0f);
		GetUIObjectByName("BackGround")->SetPositionOffset(m_pAudioOptionsPos);
		GetUIObjectByName("BackGround")->SetPosition(vec2(RelativePos.x, RelativePos.y));

		for (auto pObject : m_pAudioOptionsObjects)
		{
			pObject->SetIsOpaque(true);
		}
	}

	m_pAudio->SetChannelVolume(m_pMusicSlider->GetSliderValue(), m_pMusicSlider->GetMyPlayer());
	m_pAudio->SetChannelVolume(m_pFXSlider->GetSliderValue(), m_pFXSlider->GetMyPlayer());

	m_pAudio->PlayUIBackgroundMusic();
}

void AudioOptionsScene::Draw()
{
	for (auto pObject : m_pAudioOptionsObjects)
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

void AudioOptionsScene::OnSurfaceChange(float percentChange)
{
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	for (auto pObject : m_pAudioOptionsObjects)
	{
		vec4 RelativePos = m_pOrthoPos.GetInverse() * vec4(pObject->GetOffset().x, pObject->GetOffset().y, 0.0f, 0.0f);
		pObject->SetPosition(vec2(RelativePos.x, RelativePos.y));
		m_pTweenManager->OnSurfaceChange(pObject, m_pOrthoPos, &m_pStartingAudioPos.x, &m_pAudioOptionsPos.x);

		if (pObject->GetName() == "MusicSlider" || pObject->GetName() == "FXSlider")
		{
			pObject->OnSurfaceChange(percentChange);
		}
		if (pObject->GetIsTweening())
		{
			if (m_pTweenManager->GetTweenStateByObject(pObject) == TWEEN_SUCCESS)
			{
				pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f) * 1.4f);
			}
		}
		else
		{
			pObject->SetScale(vec3(pObject->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), pObject->GetBaseScale().y, 0.0f));
		}
	}
}

UIObject * AudioOptionsScene::GetUIObjectByName(string aName)
{
	for (auto pObject : m_pAudioOptionsObjects)
	{
		if (pObject->GetName() == aName)
		{
			return pObject;
		}
	}
	return nullptr;
}

void AudioOptionsScene::OnScenePush()
{
	for (auto pObject : m_pAudioOptionsObjects)
	{
		pObject->SetIsActive(true);
	}
}

void AudioOptionsScene::OnScenePop()
{
	for (auto pObject : m_pAudioOptionsObjects)
	{
		pObject->SetIsActive(false);
	}
}

void AudioOptionsScene::Reset()
{
	//m_pAudioOptionsPos.Set(0.975f, -1.5f);
	//m_pStartingAudioPos.Set(-0.45f, -1.0f);
	//m_pTargetScale.Set(0.0f, 0.0f, 0.0f, 0.0f);

	//m_pWindowSize = m_pGameCamera->GetWindowSize();
	//m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	//UIObject* Background = GetUIObjectByName("BackGround");
	//Background->SetPosition(m_pStartingAudioPos);
	//Background->SetPositionOffset(vec2(2.6f, 4.0f));
	//vec4 BackgroundPos = m_pOrthoPos.GetInverse() * vec4(Background->GetOffset().x, Background->GetOffset().y, 0.0, 0.0);
	//Background->SetPosition(vec2(BackgroundPos.x, BackgroundPos.y));
	//Background->SetScale(vec3(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y, 0.0f));

	//vec2 ScalarEnd = vec2(Background->GetBaseScale().x * (m_pWindowSize.x / m_pWindowSize.y), Background->GetBaseScale().y) * 1.4f;

	//m_pTweenManager->Setup2DTween(Background, TweenFunc_SineEaseInOut, m_pStartingAudioPos, m_pAudioOptionsPos, 0.0, 2.0);
	//m_pTweenManager->Setup2DScalerTween(Background, TweenFunc_SineEaseInOut, vec2(Background->GetTweenScaleCoords()->x, Background->GetTweenScaleCoords()->y), ScalarEnd, 0.0, 2.0);
	m_pTweenManager->RemoveTweensFromObject(GetUIObjectByName("BackGround"));

	for (auto pUIObject : m_pAudioOptionsObjects)
	{
		delete pUIObject;
	}

	m_pAudioOptionsObjects.clear();

	delete m_pGameCamera;

	m_pWindowSize.Set(0, 0);
	m_pOrthoPos.SetIdentity();
	m_pAudioOptionsPos.Set(0.975f, -1.5f);
	m_pStartingAudioPos.Set(-0.45f, -1.0f);
	m_pTargetScale.Set(0.0f, 0.0f, 0.0f, 0.0f);

	LoadContent();
}
