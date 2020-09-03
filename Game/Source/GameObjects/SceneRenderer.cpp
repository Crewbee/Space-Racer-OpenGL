#include "GamePCH.h"

#include "SceneRenderer.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"
#include "Mesh/Texture.h"
#include "Mesh/FBO.h"

#include "Scenes/Scene.h"

SceneRenderer::SceneRenderer(Scene * pScene, vec2 pOffset, Mesh * pMesh, Material * pMaterial)
	:UIObject(pScene, "SceneQuad", pOffset, vec2(4.7f, 3.0f), pMesh, pMaterial)
{
	m_pScene = pScene;
	m_pMesh = pMesh;
	m_pMaterial = pMaterial;
}

SceneRenderer::~SceneRenderer()
{

}

void SceneRenderer::Draw(GameCamera* aCamera)
{
	if (m_pMesh != nullptr)
	{
		m_pMesh->SetupAttributes(m_pMaterial);

		m_WorldPosition.SetIdentity();
		m_WorldPosition.CreateSRT(m_Scale, vec3(0, 0, 0), m_Position);

		m_pMesh->SetupUniformsRenderToTexture(m_pMaterial, m_WorldPosition, aCamera, 0.2f);

		m_pMesh->Draw(m_pMaterial);
	}
}

void SceneRenderer::LoadFrame(GLuint aFrame)
{
	m_pMaterial->GetTexture()->SetTexture(aFrame);
}
