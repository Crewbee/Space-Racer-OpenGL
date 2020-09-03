#include "GamePCH.h"

#include "LightingScene.h"
#include "Scenes/Scene.h"

#include "Game/Game.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/SpaceBodySpawner.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"
#include "GameObjects/Lamp.h"
#include "GameObjects/SceneRenderer.h"
#include "GameObjects/DebrisSpawner.h"
#include "GameObjects/RingSpawner.h"


#include "GameObjects/Asteroid.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"

#include "Physics3D/BulletManager.h"
#include "Physics3D/BulletDebugDraw.h"

#include "XAudio2/SoundPlayer.h"
#include "XAudio2/SoundChannel.h"
#include "XAudio2/VoiceCallback.h"
#include "XAudio2/SoundChannel.h"

#include "Events/RemoveFromSceneEvent.h"
#include "Mesh/Texture.h"
#include "Mesh/Material.h"
#include "Mesh/FBO.h"



LightingScene::LightingScene(Game * pGame, ResourceManager * pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{
	m_pSpaceShip = nullptr;
	m_pSpawner = nullptr;
	m_DoRenderToTexture = false;
	m_PlayerDead = false;
}

LightingScene::~LightingScene()
{
	delete m_pSpaceShip;
	delete m_pSpawner;
	delete m_pDebrisSpawner;
	delete m_pRingSpawner;
}

void LightingScene::LoadContent()
{
	Scene::LoadContent();

	//Create Follow Camera
	{
		m_pGameCamera = new FollowCamera(this, "Camera", vec3(0, 3, -5), vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0));
	}

	//Create Physics World and Debug Drawer
	if (m_pBulletManager == nullptr)
	{
		m_pBulletManager = new BulletManager();

		m_pDebugDrawer = new BulletDebugDraw(m_pResources, m_pGameCamera);
		m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawContactPoints);
		m_pBulletManager->SetupDebugDraw(m_pDebugDrawer);
	}

	//Create the Universe Sphere
	{
		GameObject* UniverseSphere = new GameObject(this, "UniverseSphere", UID_NULL, vec3(0, 0, 0), vec3(0, 0, 0), vec3(2.0f, 2.0f, 2.0f), m_pResources->GetMesh("UniverseMesh"), m_pResources->GetMaterial("UniverseMaterial"));
		AddGameObject(UniverseSphere);

	}
	//Create Player Ship
	{
		m_pSpaceShip = new Player(this, "SpaceShip", UID_PLAYER, vec3(0, 0, -80), vec3(0, 0, 0), vec3(1, 1, 1), m_pResources->GetMesh("ShipMesh"), m_pResources->GetMaterial("ShipMaterial"));
		m_pSpaceShip->CreateHullBody(100.0f, true);
		m_pSpaceShip->GetBody()->getWorldTransform().setOrigin(btVector3(0.0f, 0.0f, -80.0f));
		m_pSpaceShip->GetBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		m_pGameCamera->SetObjectToFollow(m_pSpaceShip, vec3(0, 1, -8));
		m_pSpaceShip->SetPlayerController(m_pResources->GetPlayerController(false));

	}

	//Create Space Body Spawner
	{
		m_pSpawner = new SpaceBodySpawner(this, m_pResources, m_pSpaceShip->GetPosition());
		m_pSpawner->LoadContent();
	}
	//Create Lights
	{
		SetHasLighting(true);

		Lamp* RedLamp = new Lamp(this, "RedLamp", Lamp::CT_PointLight, m_pSpaceShip->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f), vec3(17.0f, 6.5f, 30.0f), m_pResources->GetMesh("CubeMesh2"), m_pResources->GetMaterial("RedLightMaterial"));
		RedLamp->SetUpLight(0.0, 10, vec3(0, 0, 0), 0, 0);
		SetUpPointLights(RedLamp, 0);
		AddGameObject(RedLamp);

		Lamp* GreenLamp = new Lamp(this, "GreenLamp", Lamp::CT_PointLight, m_pSpaceShip->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f), vec3(-27.0f, 7.0f, 36.0f), m_pResources->GetMesh("CubeMesh2"), m_pResources->GetMaterial("GreenLightMaterial"));
		GreenLamp->SetUpLight(0.0, 10, vec3(0, 0, 0), 0, 0);
		SetUpPointLights(GreenLamp, 1);
		AddGameObject(GreenLamp);

		Lamp* BlueLamp = new Lamp(this, "BlueLamp", Lamp::CT_PointLight, m_pSpaceShip->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f), vec3(-9.0, -13.5f, 25.0f), m_pResources->GetMesh("CubeMesh2"), m_pResources->GetMaterial("BlueLightMaterial"));
		BlueLamp->SetUpLight(0.0, 10, vec3(0, 0, 0), 0, 0);
		SetUpPointLights(BlueLamp, 2);
		AddGameObject(BlueLamp);

		Lamp* ShipHeadLight = new Lamp(this, "HeadLight", Lamp::CT_SpotLight, m_pSpaceShip->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.35f, 0.0f), m_pResources->GetMesh("CubeMesh2"), m_pResources->GetMaterial("WhiteLightMaterial"));
		ShipHeadLight->SetUpLight(0.0, 9, vec3(0.0f, 0.0f, -1.0f), 20.0f, 25.0f);
		SetUpSpotLights(ShipHeadLight, 0);
		ShipHeadLight->TurnOffLight(true);
		AddGameObject(ShipHeadLight);
	}
	//Create the Particle Emitter
	{
		m_pDebrisSpawner = new DebrisSpawner(this, m_pResources->GetMesh("AsteroidMesh"), m_pResources->GetMaterial("AsteroidMaterial"), m_pResources);
	}
	//Create the Ring Emitter
	{
		m_pRingSpawner = new RingSpawner(this, m_pResources);
	}
}

void LightingScene::OnEvent(Event * pEvent)
{
	Scene::OnEvent(pEvent);

	m_pResources->GetPlayerController(false)->OnEvent(pEvent);

	if (pEvent->GetEventType() == GameEventType_RemoveFromScene)
	{
		RemoveFromSceneEvent* pRemoveFromSceneEvent = static_cast<RemoveFromSceneEvent*>(pEvent);

		Scene* pScene = pRemoveFromSceneEvent->GetScene();
		GameObject* pGameObject = pRemoveFromSceneEvent->GetGameObject();

		if (pGameObject->GetName() == "SpaceShip")
		{
			RemovePlayer();
		}

		if (pScene->IsGameObjectInScene(pGameObject))
		{
			pGameObject->SetIsSetImpulse(true);
			pGameObject->RemoveFromScene();
		}
	}
	if (pEvent->GetEventType() == EventType_Input)
	{
		InputEvent* pevent = static_cast<InputEvent*>(pEvent);
		if (pevent->GetInputDeviceType() == InputDeviceType_Keyboard && pevent->GetID() == VK_SHIFT)
		{
			if (pevent->GetInputState() == InputState_Pressed)
			{
				if (GetGameObjectByName("HeadLight") != nullptr)
				{
					bool turnOnHeadLights = !GetGameObjectByName("HeadLight")->GetIsLightOff();
					GetGameObjectByName("HeadLight")->TurnOffLight(turnOnHeadLights);
				}
			}
		}
	}

}

void LightingScene::Update(float deltaTime)
{
	Scene::Update(deltaTime);

	m_pSpaceShip->Update(deltaTime);

	m_pGameCamera->Update(deltaTime);

	m_pRingSpawner->Update(deltaTime);

	if (m_pSceneManager->GetGameState() == Game_State::GS_RUNNING || m_pSceneManager->GetOnDeathTimer() > 0.0f)
	{
		m_pSpawner->Update(deltaTime);


		for (auto pObject : m_pGameObjects)
		{
			pObject->Update(deltaTime);

			if (pObject->GetName() == "UniverseSphere")
			{
				pObject->SetPosition(m_pSpaceShip->GetPosition());
			}
		}

		m_pDebrisSpawner->Update(deltaTime);

		for (int i = 0; i < 3; i++)
		{
			SetUpPointLights(m_pPointLights[i], i);
		}
		if (m_pResources->GetPlayerController(false)->IsHeld_Down())
		{
			m_pDoDebugDraw = true;
		}
		else
		{
			m_pDoDebugDraw = false;
		}
	}
	m_pAudio->PlayBackGroundMusic();
}

void LightingScene::Draw()
{
	CheckForGLErrors();


	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	GetGameObjectByName("UniverseSphere")->Draw(m_pGameCamera);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	m_pSpaceShip->Draw(m_pGameCamera);

	m_pDebrisSpawner->Draw(m_pGameCamera);

	m_pRingSpawner->Draw(m_pGameCamera);

	// Render all of the Scene objects in the list.
	for (auto pObject : m_pGameObjects)
	{
		if (pObject->GetName() != "UniverseSphere")
		{
			pObject->Draw(m_pGameCamera);
		}
	}
	if (m_pDoDebugDraw)
	{
		if (m_pDebugDrawer)
		{
			m_pBulletManager->GetWorld()->debugDrawWorld();

		}
	}

	CheckForGLErrors();

}

void LightingScene::RemovePlayer()
{
	m_pSpaceShip->SetIsActive(false);
	btRigidBody* playerBody = m_pSpaceShip->GetBody();

	playerBody->setActivationState(5);
	m_pBulletManager->GetWorld()->removeCollisionObject(playerBody);

}

void LightingScene::Reset()
{
	//m_pSpaceShip->Reset();
	//m_pDebrisSpawner->Reset();
	//m_pSpawner->Reset();
	//m_pRingSpawner->Reset();
	//
	//for (auto pObject : m_pGameObjects)
	//{
	//	pObject->Reset();

	//	if (pObject->GetName() == "UniverseSphere")
	//	{
	//		pObject->SetPosition(m_pSpaceShip->GetPosition());
	//	}
	//}

	//m_PlayerDead = false;

	//m_pGameCamera->Reset();

	delete m_pSpaceShip;
	delete m_pDebrisSpawner;
	delete m_pRingSpawner;
	delete m_pSpawner;
	for (auto pObject : m_pGameObjects)
	{
		delete pObject;
	}

	m_pGameObjects.clear();

	delete m_pGameCamera;

	m_pSpaceShip = nullptr;
	m_pSpawner = nullptr;
	m_DoRenderToTexture = false;
	m_PlayerDead = false;

	LoadContent();

}

