#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "Planetoid.h"
#include "GameObjects/Player.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/DebrisSpawner.h"

#include "Scenes/Scene.h"
#include "Scenes/LightingScene.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"

#include "Physics3D/BulletManager.h"

#include "Game/Game.h"

#include "Events/RemoveFromSceneEvent.h"
#include "Events/SoundEvent.h"
#include "Events/AddScoreEvent.h"

Planetoid::Planetoid(Scene * pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, ResourceManager* pResources)
	:GameObject(pScene, name, aUID, pos, rot, scale, pMesh, nullptr)
{
	m_pResources = pResources;
	char tempstr[30];
	int RandSkin = rand() % 8;
	sprintf_s(tempstr, 30, "PlanetoidMat%d", RandSkin);
	m_pMaterial = m_pResources->GetMaterial(tempstr);
	
	m_ImpulseDirection = vec3(0.2f, 0.5f, 0.0f);

	AddRotation(m_ImpulseDirection, 0.4f);

	m_SetImpulse = true;

	m_pHealth = 2;
}

Planetoid::~Planetoid()
{

}

void Planetoid::OnEvent(Event * pEvent)
{
}

void Planetoid::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	if (m_SetImpulse == true)
	{
		m_pBody->applyImpulse(btVector3(0.0f, 0.0f, m_pBody->getInvMass() * -10.0f), m_pBody->getCenterOfMassPosition());
		m_SetImpulse = false;
	}

	if ((m_Position.z + GetMeshRadius()) < m_pScene->GetPlayer()->GetPosition().z)
	{
		Event* r = new RemoveFromSceneEvent(m_pScene, this);
		m_pScene->GetGame()->GetEventManager()->QueueEvent(r);
	}
}

bool Planetoid::RemoveFromScene()
{
	if (m_pScene->RemoveGameObject(this))
	{
		m_pBody->setActivationState(5);
		m_pBody->clearForces();
		m_pScene->GetBulletManager()->GetWorld()->removeCollisionObject(m_pBody);

		if (m_MyPool)
		{
			m_MyPool->ReturnObject(this);
		}
		return true;
	}
	return false;
}

void Planetoid::SetImpulseDirection(vec3 aDirection)
{
	m_ImpulseDirection = aDirection;
}

void Planetoid::SetPool(ObjectPool<GameObject*>* aPool)
{
	m_MyPool = aPool;
}

void Planetoid::ApplyDamage(int anAmount)
{
	m_pHealth -= anAmount;

	if (m_pHealth <= 0)
	{
		Destroy();
	}
}

void Planetoid::Destroy()
{
	Event* event = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(event);

	Event* sound = new SoundEvent(Explosion_1, CanPlay);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(sound);

	Event* score = new AddScoreEvent(150);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(score);

	m_pScene->GetDebrisSpawner()->SpawnDebris(m_Position, GetMeshRadius());
}

void Planetoid::Reset()
{
	Event* event = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(event);
}
