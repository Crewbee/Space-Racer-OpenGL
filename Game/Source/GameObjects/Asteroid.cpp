#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "Asteroid.h"
#include "GameObjects/Player.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/DebrisSpawner.h"

#include "Scenes/Scene.h"
#include "Scenes/LightingScene.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Game/Game.h"

#include "Physics3D/BulletManager.h"

#include "Events/RemoveFromSceneEvent.h"
#include "Events/SoundEvent.h"
#include "Events/AddScoreEvent.h"

Asteroid::Asteroid(Scene * pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, Material * pMaterial)
	:GameObject(pScene, name, aUID, pos, rot, scale, pMesh, pMaterial)
{
	m_ImpulseDirection = vec3(0.4f, 0.7f, 0.01f); // initialize the rotation / direction vector

	AddRotation(m_ImpulseDirection, 0.2f); //apply the rotation and a speed

	m_SetImpluse = true; //set so rotation will be applied every frame

	m_pHealth = 1; //add a health amount
}

Asteroid::~Asteroid()
{

}

void Asteroid::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	if (m_SetImpluse == true) //if impulse is set
	{
		//add impulse force in the direction of the Impulse Direction vector
		m_pBody->applyImpulse(btVector3(0.0f, 0.0f, m_pBody->getInvMass() * -10.0f), m_pBody->getCenterOfMassPosition());
	}

	if ((m_Position.z + GetMeshRadius()) < m_pScene->GetPlayer()->GetPosition().z) // if its behind the player
	{
		Event* r = new RemoveFromSceneEvent(m_pScene, this); //remove it from the scene
		m_pScene->GetGame()->GetEventManager()->QueueEvent(r);
	}
}

bool Asteroid::RemoveFromScene()
{
	if (m_pScene->RemoveGameObject(this)) //if the scene has removed the object
	{
		m_SetImpluse = true; //reset the impulse state
		m_pBody->setActivationState(5); //deactivate the physics simulation on the physics body
		m_pBody->clearForces(); //clear the current forces on the physics 
		m_pScene->GetBulletManager()->GetWorld()->removeCollisionObject(m_pBody); //remove the body from the collision world

		if (m_MyPool) //if the pool is not nullptr
		{
			m_MyPool->ReturnObject(this); //return this to the pool
		}
		return true;
	}
	return false;
}

void Asteroid::SetImpulseDirection(vec3 aDirection)
{
	m_ImpulseDirection = aDirection;
}

void Asteroid::SetPool(ObjectPool<GameObject*>* aPool)
{
	m_MyPool = aPool;
}

void Asteroid::ApplyDamage(int anAmount)
{
	m_pHealth -= anAmount;

	if (m_pHealth <= 0)
	{
		Destroy();
	}
}

void Asteroid::Destroy()
{
	//send an event to remove the object from the scene
	Event* event = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(event);

	//send an event to make an explosion sound
	Event* sound = new SoundEvent(Explosion_1, CanPlay);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(sound);

	//send an event to add score to the score board
	Event* score = new AddScoreEvent(50);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(score);

	//get the debris spawner to spawn debris
	m_pScene->GetDebrisSpawner()->SpawnDebris(m_Position, GetMeshRadius());
}

void Asteroid::Reset()
{
	//send an event to remove the object from the scene
	Event* event = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(event);
}
