#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/GameCamera.h"
#include "OrthoCamera.h"

#include "Helpers/TweenManager.h"

#include "Game/Game.h"

#include "Scenes/Scene.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

OrthoCamera::OrthoCamera(Scene * pScene, std::string name, vec3 pos)
	:GameCamera(pScene, name, pos, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0 ,0))
{

}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::OnEvent(Event * pEvent)
{
}

void OrthoCamera::Update(float deltaTime)
{

}

void OrthoCamera::OnSurfaceChange(float angle, vec2 aspectRatio, float Znear, float Zfar)
{
	float OrthoWidth = aspectRatio.x / aspectRatio.y;
	m_pOrthoVector = vec4(-OrthoWidth, OrthoWidth, -1.0f, 1.0f);
	m_pProjMatrix.CreateOrtho(m_pOrthoVector.x, m_pOrthoVector.y, m_pOrthoVector.z, m_pOrthoVector.w, 0.0f, 1.0f);

	float percentChange = aspectRatio.x / m_WindowSize.x;

	m_WindowSize = aspectRatio;

	m_pScene->OnSurfaceChange(percentChange);
}

vec4 OrthoCamera::GetOrthoVector()
{
	return m_pOrthoVector;
}

void OrthoCamera::SetOrthoProj(vec4 anOrthoVector)
{
	m_pProjMatrix.CreateOrtho(anOrthoVector.x, anOrthoVector.y, anOrthoVector.z, anOrthoVector.w, 0.0f, 1.0f);
	m_pOrthoVector = anOrthoVector;
}

