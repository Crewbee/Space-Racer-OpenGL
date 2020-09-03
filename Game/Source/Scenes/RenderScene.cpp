#include "GamePCH.h"

#include "Scenes/Scene.h"
#include "RenderScene.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/SceneRenderer.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/SceneManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"

#include "Game/Game.h"

RenderScene::RenderScene(Game * pGame, ResourceManager * pResources, AudioController * pAudioController, SceneManager * aSceneManager, TweenManager * pTweenManager)
	:Scene(pGame, pResources, aSceneManager, pAudioController, pTweenManager)
{
	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();
}

RenderScene::~RenderScene()
{
	delete m_pRenderObject;
}

void RenderScene::LoadContent()
{
	m_pGameCamera = new OrthoCamera(this, "OrthoCam", vec3(0, 0, 0));
	m_pWindowSize = m_pGameCamera->GetWindowSize();
	m_pOrthoPos = *m_pGameCamera->GetProjMatrix();

	m_pRenderMesh = m_pResources->GetMesh("RenderMesh");

	vec4 MeshSize = m_pGameCamera->GetOrthoVector();

	m_pRenderMesh->ClearBuffers();
	m_pRenderMesh->CreateBox(vec2((MeshSize.y / MeshSize.w) - (MeshSize.w / MeshSize.y) - 0.41f, (MeshSize.w / MeshSize.y) * 2.2f), vec2(0.0f, 0.0f));

	m_pRenderObject = new SceneRenderer(this, vec2(m_pRenderMesh->GetMeshSize().x / 2 - 0.335, m_pRenderMesh->GetMeshSize().y / 2 + 0.245), m_pRenderMesh, m_pResources->GetMaterial("RenderMaterial"));
	vec4 m_pRenderObjectPos = m_pOrthoPos.GetInverse() * vec4(m_pRenderObject->GetOffset().x, m_pRenderObject->GetOffset().y, 0.0, 0.0);
	m_pRenderObject->SetPosition(vec2(m_pRenderObjectPos.x, m_pRenderObjectPos.y));
}

void RenderScene::OnEvent(Event * pEvent)
{

}

void RenderScene::Update(float deltaTime)
{

}

void RenderScene::Draw()
{
	m_pRenderObject->Draw(m_pGameCamera);
}

void RenderScene::OnSurfaceChange(float percentage)
{

}

void RenderScene::OnScenePush()
{
}

void RenderScene::OnScenePop()
{
}

void RenderScene::Reset()
{

	delete m_pRenderObject;

	delete m_pGameCamera;

	m_pWindowSize.Set(0.0f, 0.0f);
	m_pOrthoPos.SetIdentity();

	LoadContent();
}
