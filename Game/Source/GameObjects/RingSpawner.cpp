#include "GamePCH.h"

#include "RingSpawner.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/SpaceRing.h"
#include "GameObjects/Player.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"
#include "Helpers/SceneManager.h"

#include "Mesh/Material.h"
#include "Mesh/Mesh.h"

#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"

#include "Scenes/Scene.h"

RingSpawner::RingSpawner(Scene * pScene, ResourceManager * pResources)
	:m_RingPool()
{
	m_pResources = pResources;
	m_pMyScene = pScene;
	for (int i = 0; i < MaxDebris; i++)
	{
		char tempstr[30];
		sprintf_s(tempstr, 30, "Ring%d", i);

        SpaceRing* aRing = new SpaceRing(m_pMyScene, tempstr, UID_NULL, vec3(0, 0, 0), vec3(0, 0, 0), vec3(30.0f, 30.0f, 40.0f), m_pResources->GetMesh("RingMesh"), m_pResources->GetMaterial("RingMaterial"), this);
		m_RingPool.AddNewObjectToPool(aRing);
		aRing->SetPool(&m_RingPool);
	}

	m_pFinishRing = new SpaceRing(m_pMyScene, "Finish", UID_NULL, vec3(0, 0, 100000), vec3(0, 0, 0), vec3(30.0f, 30.0f, 40.0f), m_pResources->GetMesh("FinishLineMesh"), m_pResources->GetMaterial("FinishLineMaterial"), this);

	m_pRingSpacing = 400.0f;
	m_pStartingSpawnTimer = 0.2f;
	m_pPlayerZ = -80.0f;
}

RingSpawner::~RingSpawner()
{
	for (auto pDebris : m_pActiveRings)
	{
		delete pDebris;
	}
	delete m_pFinishRing;
}

void RingSpawner::OnEvent(Event * pEvent)
{
}

void RingSpawner::Update(float deltaTime)
{
	if (m_pMyScene->GetSceneManager()->GetGameState() == Game_State::GS_BEGINPLAY)
	{
		m_pStartingSpawnTimer -= deltaTime;
		if (m_pActiveRings.size() < 15)
		{
			if (m_pStartingSpawnTimer < 0.0f)
			{
				SpawnRing(vec3(0.0f, 0.0f, m_pRingSpacing + (m_pRingSpacing * m_pActiveRings.size())));
				m_pStartingSpawnTimer = 0.2f;
			}
		}

	}
	if (m_pMyScene->GetSceneManager()->GetGameState() == Game_State::GS_RUNNING)
	{
		m_pPlayerZ = m_pMyScene->GetPlayer()->GetPosition().z;

		if (m_pActiveRings.front()->GetPosition().z < m_pPlayerZ)
		{
			m_pActiveRings.front()->RemoveFromScene();
		}

		if (m_pActiveRings.size() < 15)
		{
			SpawnRing(vec3(0.0f, 0.0f, m_pActiveRings.back()->GetPosition().z + m_pRingSpacing));
		}

		if (m_pPlayerZ > m_pFinishRing->GetPosition().z)
		{
			m_pMyScene->GetPlayer()->OnPlayerWin();
		}

	}
	m_pFinishRing->Update(deltaTime);
}

void RingSpawner::Draw(GameCamera * aCamera)
{
	if (m_pFinishRing->GetPosition().z - m_pPlayerZ < 8000.0f)
	{
		m_pFinishRing->Draw(aCamera);
	}
	for (auto pRing : m_pActiveRings)
	{
		pRing->Draw(aCamera);
	}
}

void RingSpawner::SpawnRing(vec3 aPosition)
{
	GameObject* ring = m_RingPool.GetObjectFromPool();
	if (ring)
	{
		ring->SetPosition(aPosition);
		m_pActiveRings.push_back(ring);
	}
}

bool RingSpawner::RemoveRing(GameObject * aRing)
{
	auto iteratorForObject = std::find(m_pActiveRings.begin(), m_pActiveRings.end(), aRing);
	if (iteratorForObject != m_pActiveRings.end())
	{
		m_pActiveRings.erase(iteratorForObject);
		return true;
	}
	return false;
}

bool RingSpawner::RingIsActive(GameObject * aRing)
{
	auto iteratorForObject = std::find(m_pActiveRings.begin(), m_pActiveRings.end(), aRing);
	if (iteratorForObject != m_pActiveRings.end())
	{
		return true;
	}
	return false;
}

void RingSpawner::Reset()
{
	for (auto pRing : m_pActiveRings)
	{
        if (pRing->RemoveFromScene())
        {
            continue;
        }
	}
	m_pStartingSpawnTimer = 0.2f;
	m_pPlayerZ = -80.0f;

    m_pFinishRing->SetPosition(vec3(0.0f, 0.0f, 100000.0f));
}
