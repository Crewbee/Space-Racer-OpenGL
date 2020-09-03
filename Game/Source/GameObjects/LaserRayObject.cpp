#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "LaserRayObject.h"
#include "GameObjects/Player.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"

#include "Scenes/Scene.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/AudioController.h"

#include "Game/Game.h"

LaserRayObject::LaserRayObject(Scene * pScene, string name, vec3 pos, vec3 rot, vec3 scale, Mesh * pMesh, Material * pMaterial, Player * pPlayer)
	:GameObject(pScene, name, UID_NULL, pos, rot, scale, pMesh, pMaterial)
{
	m_pLaserWidth.Set(2.0f, 2.0f);
	m_pLifeTime = 2.0f;

	m_pMesh = new Mesh();
	m_pMaterial = pScene->GetResourceManager()->GetMaterial("RedLightMaterial");
	m_pDrawLaser = false;
	m_pMyPlayer = pPlayer;
}

LaserRayObject::~LaserRayObject()
{
	delete m_pMesh;
}

void LaserRayObject::OnEvent(Event * pEvent)
{

}

void LaserRayObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	if (m_pDrawLaser)
	{
		m_pLifeTime -= deltaTime;
	}
	if (m_pLifeTime <= 0.0f)
	{
		m_pDrawLaser = false;
		m_pLifeTime = 0.0f;
	}
	m_Position = m_pMyPlayer->GetPosition();
}

void LaserRayObject::Draw(GameCamera * aCamera)
{
	if (m_pMesh)
	{
		if (m_pDrawLaser)
		{
			m_pMesh->SetupAttributes(m_pMaterial);

			m_WorldPosition.SetIdentity();

			m_WorldPosition.CreateSRT(vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), m_pMuzzle);

			float anAlpha = m_pLifeTime / 2.0f;

			m_pMesh->SetupLaserUniforms(m_pMaterial, m_WorldPosition, aCamera, anAlpha);

			m_pMesh->Draw(m_pMaterial);
		}
	}
}

bool LaserRayObject::InitLaser(vec3 aLocation)
{
	if (m_pMesh)
	{
		m_pMesh->ClearBuffers();

		vec3 LaserDestination = vec3(aLocation.x - m_pMuzzle.x, aLocation.y - m_pMuzzle.y, aLocation.z - m_pMuzzle.z);

		m_pMesh->CreateLaserPlane(m_pLaserWidth, LaserDestination, vec3(0.0, 0.0, 0.0));

		return true;
	}
	return false;
}

void LaserRayObject::FireLaser(vec3 aLocation)
{
	if (InitLaser(aLocation))
	{
		m_pDrawLaser = true;
		m_pLifeTime = 2.0f;
	}
}
