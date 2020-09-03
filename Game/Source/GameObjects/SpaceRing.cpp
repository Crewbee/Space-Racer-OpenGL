#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "SpaceRing.h"
#include "GameObjects/Player.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/RingSpawner.h"

#include "Scenes/Scene.h"
#include "Scenes/LightingScene.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Game/Game.h"

#include "Physics3D/BulletManager.h"

#include "Events/RemoveFromSceneEvent.h"

SpaceRing::SpaceRing(Scene * pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, Material * pMaterial, RingSpawner* pDebrisSpawner)
	:GameObject(pScene, name, aUID, pos, rot, scale, pMesh, pMaterial)
{
	m_pMySpawner = pDebrisSpawner;
}

SpaceRing::~SpaceRing()
{

}

void SpaceRing::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void SpaceRing::Draw(GameCamera * aCamera)
{
	m_pMesh->SetupAttributes(m_pMaterial);

	m_WorldPosition.SetIdentity();
	m_WorldPosition.CreateSRT(m_Scale, vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z), m_Position);
	mat4 NormalMatrix;
	NormalMatrix.CreateRotation(vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));

	m_pMesh->SetupUniformsWithLightForDebris(m_pMaterial, m_WorldPosition, NormalMatrix, aCamera, m_pScene->GetPointLights(), m_pScene->GetSpotLights(), 1.0f);

	m_pMesh->Draw(m_pMaterial);
}

bool SpaceRing::RemoveFromScene()
{
	if (m_pMySpawner->RemoveRing(this))
	{
		if (m_MyPool)
		{
			m_MyPool->ReturnObject(this);
			return true;
		}
	}
	return false;
}

void SpaceRing::SetPool(ObjectPool<GameObject*>* aPool)
{
	m_MyPool = aPool;
}

void SpaceRing::Destroy()
{
	Event* event = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(event);
}