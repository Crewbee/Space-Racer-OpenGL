#include "GamePCH.h"

#include "SceneManager.h"
#include "Scenes/Scene.h"
#include "Scenes/LightingScene.h"
#include "Scenes/SplashScene.h"
#include "Scenes/MenuScene.h"
#include "Scenes/OptionScene.h"
#include "Scenes/HUDScene.h"
#include "Scenes/AudioOptionsScene.h"
#include "Scenes/GameOverScene.h"
#include "Scenes/RenderScene.h"
#include "Scenes/PauseScene.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"

#include "Mesh/FBO.h"

#include "GameObjects/GameCamera.h"

#include "Events/SoundEvent.h"
#include "Events/GameStateChangeEvent.h"

#include "Game/Game.h"

SceneManager::SceneManager(Game* pGame, ResourceManager* pResources, AudioController* pAudio, TweenManager* pTweenManager, FBO* pFrameBuffer)
{
	m_pGame = pGame;
	m_pResources = pResources;
	m_pAudio = pAudio;
	m_pTweenManager = pTweenManager;
	m_pFrameBufferObject = pFrameBuffer;
	m_pWindowSize.Set(1920.0f, 1080.0f);
	m_pGameOverPushTimer = 2.0f;
	m_pPlayEndSound = true;
}

SceneManager::~SceneManager()
{
	m_pActiveScenes.clear();

	for (auto pScene : m_pScenes)
	{
		delete pScene;
	}
}

void SceneManager::LoadContent()
{
	Scene* aGameScene = new LightingScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aGameScene->SetSceneType(ST_GAMEPLAY);
	aGameScene->LoadContent();

	m_pScenes.push_back(aGameScene);

	Scene* aHUDScene = new HUDScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aHUDScene->SetSceneType(ST_HUD);
	aHUDScene->LoadContent();

	m_pScenes.push_back(aHUDScene);

	Scene* aOptionScene = new OptionScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aOptionScene->SetSceneType(ST_OPTIONS);
	aOptionScene->LoadContent();

	m_pScenes.push_back(aOptionScene);

	Scene* aAudioOptionsScene = new AudioOptionsScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aAudioOptionsScene->SetSceneType(ST_AUDIO);
	aAudioOptionsScene->LoadContent();

	m_pScenes.push_back(aAudioOptionsScene);

	Scene* aMenuScene = new MenuScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aMenuScene->SetSceneType(ST_MENU);
	aMenuScene->LoadContent();

	m_pScenes.push_back(aMenuScene);

	Scene* aSplashScene = new SplashScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aSplashScene->SetSceneType(ST_SPLASH);
	aSplashScene->LoadContent();

	m_pScenes.push_back(aSplashScene);

	Scene* aPauseScene = new PauseScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aPauseScene->SetSceneType(ST_PAUSE);
	aPauseScene->LoadContent();

	m_pScenes.push_back(aPauseScene);

	Scene* aGameOverScene = new GameOverScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aGameOverScene->SetSceneType(ST_GAMEOVER);
	aGameOverScene->LoadContent();

	m_pScenes.push_back(aGameOverScene);

	Scene* aRenderScene = new RenderScene(m_pGame, m_pResources, m_pAudio, this, m_pTweenManager);
	aRenderScene->SetSceneType(ST_RENDER);
	aRenderScene->LoadContent();

	m_pScenes.push_back(aRenderScene);

	m_pCurrentGameState = GS_INMENU;
}

void SceneManager::OnEvent(Event * pEvent)
{
	if (pEvent->GetEventType() == EventType_Input)
	{
		InputEvent* pInput = static_cast<InputEvent*>(pEvent);

		if (m_pCurrentScene == ST_SPLASH)
		{
			if (pInput->GetInputDeviceType() == InputDeviceTypes::InputDeviceType_Keyboard && pInput->GetID() == VK_RETURN)
			{
				Event* paudio = new SoundEvent(ClickButton, MustPlay);
				m_pGame->GetEventManager()->QueueEvent(paudio);
				PopScene();
			}
		}
		if (pInput->GetInputDeviceType() == InputDeviceTypes::InputDeviceType_Keyboard && pInput->GetID() == VK_TAB)
		{
			if (pInput->GetInputState() == InputStates::InputState_Pressed)
			{
				if (m_pCurrentScene == ST_HUD)
				{
					m_pActiveScenes.pop_back();
					m_pAudio->StopMusic();
					m_pAudio->StopSounds();
					PushScene(ST_PAUSE);
				}
				else if (m_pCurrentScene == ST_PAUSE)
				{
					m_pActiveScenes.pop_back();
					PushScene(ST_HUD);
				}
			}
		}

	}
	if (pEvent->GetEventType() == GameEventType_GameStateChange)
	{
		GameStateChangeEvent* pChangeState = static_cast<GameStateChangeEvent*>(pEvent);

		m_pCurrentGameState = pChangeState->GetState();

		for (int i = 0; i < m_pScenes.size(); i++)
		{
			m_pScenes.at(i)->OnEvent(pChangeState);
		}
	}
	if (m_pCurrentScene == ST_HUD || m_pCurrentScene == ST_PAUSE)
	{
		GetScene(ST_GAMEPLAY)->OnEvent(pEvent);
		GetScene(ST_HUD)->OnEvent(pEvent);
	}
	else
	{
		m_pActiveScenes.back()->OnEvent(pEvent);
	}
}

void SceneManager::Update(float deltaTime)
{
	if (m_pCurrentGameState == GS_GAMEOVER)
	{
		m_pGameOverPushTimer -= deltaTime;

		if (m_pGameOverPushTimer < 0.0f)
		{
			if (m_pPlayEndSound == true)
			{
				m_pAudio->StopMusic();
				m_pAudio->StopSounds();
				Event* sound = new SoundEvent(Announcer_Failure, MustPlay);
				m_pGame->GetEventManager()->QueueEvent(sound);

				m_pPlayEndSound = false;
			}

			PopScene();
			PushScene(ST_GAMEOVER);
		}
	}
	if (m_pCurrentGameState == GS_WINGAME)
	{
		m_pGameOverPushTimer -= deltaTime;

		if (m_pGameOverPushTimer < 0.0f)
		{
			if (m_pPlayEndSound == true)
			{
				m_pAudio->StopMusic();
				m_pAudio->StopSounds();
				Event* sound = new SoundEvent(Announcer_Success, MustPlay);
				m_pGame->GetEventManager()->QueueEvent(sound);

				m_pPlayEndSound = false;
			}

			PopScene();
			PushScene(ST_GAMEOVER);
		}
	}
	if (m_pCurrentScene == ST_HUD)
	{

		GetScene(ST_GAMEPLAY)->Update(deltaTime);
		GetScene(ST_HUD)->Update(deltaTime);

	}
	else
	{
		m_pActiveScenes.back()->Update(deltaTime);
	}
}

void SceneManager::Draw()
{
	if (m_pCurrentScene == ST_HUD)
	{
		m_pFrameBufferObject->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GetScene(ST_GAMEPLAY)->Draw();

		m_pFrameBufferObject->Unbind();


		GetScene(ST_RENDER)->Draw();

		GetScene(ST_HUD)->Draw();
	}
	else if (m_pCurrentScene == ST_PAUSE)
	{
		m_pFrameBufferObject->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GetScene(ST_GAMEPLAY)->Draw();

		m_pFrameBufferObject->Unbind();


		GetScene(ST_RENDER)->Draw();
		GetScene(ST_PAUSE)->Draw();
	}
	else if (m_pCurrentScene == ST_GAMEOVER)
	{
		m_pFrameBufferObject->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GetScene(ST_GAMEPLAY)->Draw();

		m_pFrameBufferObject->Unbind();


		GetScene(ST_RENDER)->Draw();
		GetScene(ST_GAMEOVER)->Draw();
	}
	else
	{
		m_pActiveScenes.back()->Draw();
	}
}

void SceneManager::PopScene()
{
	if (m_pActiveScenes.size() > 0)
	{
		m_pActiveScenes.back()->OnScenePop();
		m_pActiveScenes.back()->GetCamera()->SetIsActive(false);
		m_pActiveScenes.pop_back();
	}
	m_pCurrentScene = m_pActiveScenes.back()->GetType();
	m_pActiveScenes.back()->OnScenePush();
	m_pActiveScenes.back()->GetCamera()->SetIsActive(true);
}

void SceneManager::PushScene(Scene_Types aType)
{
	for (auto pScene : m_pScenes)
	{
		if (pScene->GetType() == aType)
		{
			m_pActiveScenes.push_back(pScene);
			m_pCurrentScene = aType;
			pScene->OnScenePush();
			pScene->GetCamera()->SetIsActive(true);
		}
		else
		{
			pScene->OnScenePop();
			pScene->GetCamera()->SetIsActive(false);
		}
	}
}

void SceneManager::ResetFOV(float angle, vec2 aspectRatio, float Znear, float Zfar)
{
	for (auto pScene : m_pScenes)
	{
		pScene->GetCamera()->OnSurfaceChange(angle, aspectRatio, Znear, Zfar);
	}

	m_pWindowSize = aspectRatio;
}

Scene * SceneManager::GetScene(Scene_Types aType)
{
	for (auto pScene : m_pScenes)
	{
		if (pScene->GetType() == aType)
		{
			return pScene;
		}
	}
	return nullptr;
}

void SceneManager::ResetGame()
{
	for (auto pScene : m_pScenes)
	{
		pScene->Reset();
	}
	m_pGameOverPushTimer = 1.0f;
	m_pPlayEndSound = true;
	m_pCurrentGameState = GS_INMENU;
	m_pActiveScenes.clear();

	PushScene(ST_GAMEPLAY);
	PushScene(ST_HUD);
	PushScene(ST_MENU);
	PushScene(ST_SPLASH);

    m_pAudio->Reset();
}
