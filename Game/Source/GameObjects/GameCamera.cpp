#include "GamePCH.h"

#include "GameCamera.h"
#include "Game/Game.h"
#include "Scenes/Scene.h"

#include "Mesh/FBO.h"

GameCamera::GameCamera(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, vec3 aTarget)
	: GameObject(pScene, name, UID_NULL, pos, rot, scale, 0, 0)
{
	if (name == "OrthoCam")
	{
		m_pViewMatrix.SetIdentity();
		m_WindowSize = vec2(1920.0f, 1080.0f);
		m_pProjMatrix.CreateOrtho(-(m_WindowSize.x / m_WindowSize.y), (m_WindowSize.x / m_WindowSize.y), -1.0f, 1.0f, 0.0f, 1.0f);

		m_pOrthoVector.Set(-(m_WindowSize.x / m_WindowSize.y), (m_WindowSize.x / m_WindowSize.y), -1.0f, 1.0f);

	}
	else
	{
		m_pCamNear = 0.01f;
		m_pCamFar = 1000.0f;
		m_pCamDist = -30.0f;

		m_pLookAtPosition.Set(0, 0, 0);
		m_pStartTarget = aTarget;;
		m_pUpVector = vec3(0, 1, 0);

		m_pViewMatrix.CreateLookAtView(vec3(m_Position.x, m_Position.y, m_pCamDist), m_pUpVector, aTarget);
		m_WindowSize = vec2(1920.0f, 1080.0f);
		m_pProjMatrix.CreatePerspectiveVFoV(45.0f, m_WindowSize.x / m_WindowSize.y, 50.0f, 50000.0f);

		m_pOrthoVector.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

GameCamera::~GameCamera()
{
}

void GameCamera::Update(float deltaTime)
{
	// Setup camera/view matrix.
	m_pViewMatrix.CreateLookAtView(m_Position, vec3(0, 1, 0), m_pLookAtPosition);

	float windowWidth = (float)m_pScene->GetGame()->GetFramework()->GetWindowWidth();
	float windowHeight = (float)m_pScene->GetGame()->GetFramework()->GetWindowHeight();

	m_pProjMatrix.CreatePerspectiveVFoV(45.0f, windowWidth / windowHeight, 0.01f, 5000.0f);


}
void GameCamera::SetupViewLookAt(vec3 aPosition, vec3 anUpVector, vec3 aTarget)
{
	m_pViewMatrix.CreateLookAtView(aPosition, anUpVector, aTarget);
}
void GameCamera::OnSurfaceChange(float angle, vec2 aspectRatio, float Znear, float Zfar)
{
	m_WindowSize = aspectRatio;

	//if (m_pScene->GetSceneManager()->GetFrameBuffer())
	//{
	//	m_pScene->GetSceneManager()->GetFrameBuffer()->OnSurfaceChange(ivec2(aspectRatio.x, aspectRatio.y));
	//}
	float AspectPercentageChanged = aspectRatio.x / aspectRatio.y * 0.5f;
	m_pProjMatrix.CreatePerspectiveVFoV(angle * AspectPercentageChanged, aspectRatio.x / aspectRatio.y, Znear, Zfar);

	float ZDistance = m_pCamDist;

	ZDistance *= AspectPercentageChanged;
	SetupViewLookAt(vec3(m_Position.x, m_Position.y, ZDistance), m_pUpVector, m_pStartTarget);
}

