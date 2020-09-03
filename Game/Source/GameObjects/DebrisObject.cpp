#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "DebrisObject.h"
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

DebrisObject::DebrisObject(Scene * pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, Material * pMaterial, DebrisSpawner* pDebrisSpawner)
	:GameObject(pScene, name, aUID, pos, rot, scale, pMesh, pMaterial)
{
	m_ImpulseDirection = vec3(0.0f, 0.0f, 0.0f); // initialize the rotation / direction vector

	AddRotation(m_ImpulseDirection, 0.2f); //apply the rotation and a speed

	m_pLifeTime = 2.0f; //Set the initial life time of the debris

	m_pMySpawner = pDebrisSpawner; //add a reference to the debris spawner
}

DebrisObject::~DebrisObject()
{

}

void DebrisObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	m_pLifeTime -= deltaTime; // Tick down the life time every frame
	m_Position += m_ImpulseDirection * deltaTime; //apply a frame based increment to the position based on the Impulse Direction

	if ((m_Position.z + GetMeshRadius()) < m_pScene->GetPlayer()->GetPosition().z) //if the debris is behind the player
	{
		m_pLifeTime = 0.0f; // set its life time to 0
	}
}

void DebrisObject::Draw(GameCamera * aCamera)
{
	m_pMesh->SetupAttributes(m_pMaterial); //set up the mesh's attributes (position) into the mesh's VBO

	 //reset the world matrix to identity
	m_WorldPosition.SetIdentity();

	//create a Scale/Rotation/Translation matrix base on the debris members
	m_WorldPosition.CreateSRT(m_Scale, vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z), m_Position);
	//Create a matrix for the normals of the the mesh
	mat4 NormalMatrix;
	NormalMatrix.CreateRotation(vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));

	float alpha = m_pLifeTime / 2.0f; //create a Alpha channel based on the remaining life time

	//Setup the uniforms for the Debris
	m_pMesh->SetupUniformsWithLightForDebris(m_pMaterial, m_WorldPosition, NormalMatrix, aCamera, m_pScene->GetPointLights(), m_pScene->GetSpotLights(), alpha);

	//call draw on the mesh
	m_pMesh->Draw(m_pMaterial);
}

bool DebrisObject::RemoveFromScene()
{
	if (m_pMySpawner->RemoveDebris(this)) //if the spawner has deleted the debris
	{
		//Return it to its Object Pool
		if (m_MyPool)
		{
			m_MyPool->ReturnObject(this);
			return true;
		}
	}
	return false;
}

void DebrisObject::SetImpulseDirection(vec3 aDirection)
{
	m_ImpulseDirection = aDirection;
}

void DebrisObject::SetPool(ObjectPool<GameObject*>* aPool)
{
	m_MyPool = aPool;
}

void DebrisObject::Destroy()
{
	//Send an event to remove the debris from the scene
	Event* event = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(event);
}
