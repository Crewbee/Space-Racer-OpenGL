#include "GamePCH.h"

#include "Scene.h"

#include "Game/Game.h"
#include "Game/ObjectPool.h"

#include "Events/GameEventTypes.h"
#include "Events/RemoveFromSceneEvent.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/SceneManager.h"
#include "Helpers/TweenManager.h"
#include "Helpers/TweenFuncs.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/Player.h"
#include "GameObjects/Lamp.h"
#include "GameObjects/DebrisSpawner.h"

#include "Physics3D/BulletManager.h"
#include "Physics3D/BulletDebugDraw.h"

Scene::Scene(Game* pGame, ResourceManager* pResources, SceneManager* pSceneManager, AudioController* pAudio,TweenManager* pTweenManager)
{
    m_pGame = pGame;
    m_pResources = pResources;
	m_pAudio = pAudio;
	m_pSceneManager = pSceneManager;
    m_pTweenManager = pTweenManager;
	m_pBulletManager = nullptr;
	m_pDebugDrawer = nullptr;
	m_pGameCamera = nullptr;
	m_pHasLighting = false;
	m_pDoDebugDraw = false;
	m_pDebrisSpawner = nullptr;
	m_pRingSpawner = nullptr;
}

Scene::~Scene()
{
    Cleanup();
}

void Scene::Cleanup()
{
    for( auto pObject: m_pGameObjects )
    {
        delete pObject;
    }
    m_pGameObjects.clear();

	if (m_pDebugDrawer)
	{
		delete m_pDebugDrawer;
	}

	if (m_pBulletManager)
	{
		delete m_pBulletManager;
	}
	if (m_pGameCamera)
	{
		delete m_pGameCamera;
	}
}

void Scene::LoadContent()
{

}

void Scene::Reset()
{
    // Reset all of the Scene objects in the list.
    for( auto pObject: m_pGameObjects )
    {
        pObject->Reset();
    }
}

void Scene::OnEvent(Event* pEvent)
{
    // Send the event to Scene objects in the list.
    for( auto pObject: m_pGameObjects )
    {
        pObject->OnEvent( pEvent );
    }
	m_pGameCamera->OnEvent(pEvent);

	if (pEvent->GetEventType() == GameEventType_RemoveFromScene)
	{
		RemoveFromSceneEvent* pRemoveFromSceneEvent = (RemoveFromSceneEvent*)pEvent;

		GameObject* pGameObject = pRemoveFromSceneEvent->GetGameObject();
		pGameObject->RemoveFromScene();
	}
	if (pEvent->GetEventType() == EventType_Input)
	{
		InputEvent* pInput = static_cast<InputEvent*>(pEvent);
		// Reset active scene.
		if (pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 'R')
			Reset();
	}
}

void Scene::Update(float deltaTime)
{
    CheckForGLErrors();

	if (m_pBulletManager)
	{
		m_pBulletManager->Update(deltaTime);
	}

    // Update all of the Scene objects in the list.
    for( unsigned int i=0; i<m_pGameObjects.size(); i++ )
    {
        m_pGameObjects[i]->Update( deltaTime );
    }


    CheckForGLErrors();
}

void Scene::Draw()
{
    CheckForGLErrors();

    GameCamera* pCamera = static_cast<GameCamera*>( GetGameObjectByName( "Camera" ) );
    assert( pCamera );

    // Render all of the Scene objects in the list.
    for( auto pObject: m_pGameObjects )
    {
		if (pObject->GetName() != "Camera")
		{
			pObject->Draw(pCamera);
		}
    }

    CheckForGLErrors();
}

void Scene::SetUpPointLights(Lamp* aPointLight, int anIndex)
{
	assert(aPointLight->GetCasterType() == Lamp::CT_PointLight);
	if (m_pHasLighting == true)
	{
		if (m_pPointLights.size() < 3)
		{
			m_pPointLights.push_back(aPointLight);
			m_PointLightProperties.push_back(aPointLight->GetPointLighting());
		}
		else
		{
			m_pPointLights.at(anIndex) = aPointLight;
			m_PointLightProperties.at(anIndex) = aPointLight->GetPointLighting();
		}
	}
}

PointLight* Scene::GetPointLights()
{
	if (m_pHasLighting == true)
	{
		return &m_PointLightProperties[0];
	}
	return 0;
}

void Scene::SetUpSpotLights(Lamp* aSpotLight, int anIndex)
{
	assert(aSpotLight->GetCasterType() == Lamp::CT_SpotLight);

	if (m_pHasLighting == true)
	{
		if (m_pSpotLights.size() < 1)
		{
			m_pSpotLights.push_back(aSpotLight);
			m_SpotLightProperties.push_back(aSpotLight->GetSpotLighting());
		}
		else
		{
			m_pSpotLights.at(anIndex) = aSpotLight;
			m_SpotLightProperties.at(anIndex) = aSpotLight->GetSpotLighting();
		}
	}
}
void Scene::OnScenePush()
{
	for (auto pObject : m_pGameObjects)
	{
		pObject->SetIsActive(true);
	}
}
void Scene::OnScenePop()
{
	for (auto pObject : m_pGameObjects)
	{
		pObject->SetIsActive(false);
	}
}
SpotLight* Scene::GetSpotLights()
{
	if (m_pHasLighting == true)
	{
		return &m_SpotLightProperties[0];
	}
	return 0;
}

void Scene::AddGameObject(GameObject* pObject)
{
    m_pGameObjects.push_back( pObject );
}

bool Scene::RemoveGameObject(GameObject* pObject)
{
    auto iteratorForObject = std::find( m_pGameObjects.begin(), m_pGameObjects.end(), pObject );
    
    if( iteratorForObject != m_pGameObjects.end() )
    {
        m_pGameObjects.erase( iteratorForObject );
        return true;
    }

    return false;
}

bool Scene::IsGameObjectInScene(GameObject* pObject)
{
    auto iteratorForObject = std::find( m_pGameObjects.begin(), m_pGameObjects.end(), pObject );

    if( iteratorForObject != m_pGameObjects.end() )
    {
        return true;
    }

    return false;
}

GameObject* Scene::GetGameObjectByName(std::string name)
{
    for( auto pObject: m_pGameObjects )
    {
        if( pObject->GetName() == name )
        {
            return pObject;
        }
    }

    return nullptr;
}

GameObject * Scene::GetObjectAtLocation(vec3 aPosition)
{
	for (auto pObject : m_pGameObjects)
	{
		if (pObject->GetPosition() == aPosition)
		{
			return pObject;
		}
	}
	return nullptr;
}
