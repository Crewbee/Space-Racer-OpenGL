#include "GamePCH.h"

#include "GameObjects/UIObject.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"
#include "ScoreDisplay.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Scenes/Scene.h"

#include "Events/AddScoreEvent.h"
#include "Events/GameEventTypes.h"

ScoreDisplay::ScoreDisplay(Scene * pScene, std::string name, vec2 pOffset, vec2 scale, Mesh * pMesh, Material * pMaterial, ResourceManager* pResources, mat4 Ortho)
	:UIObject(pScene, name, pOffset, scale, pMesh, pMaterial)
{
	m_pResources = pResources;
	m_pOrtho = Ortho;
	m_pCurrentScore = 0;
	for (int i = 0; i < 6; i++)
	{
		m_pScoreDisplay[i] = 0;
		vec4 aScoreDisplayPos = m_pOrtho.GetInverse() * vec4(m_Position.x + (GetMeshSize().x * i), m_Position.y, 0.0, 0.0);
		m_pFramePositions.push_back(vec2(aScoreDisplayPos.x, aScoreDisplayPos.y));

	}

}

ScoreDisplay::~ScoreDisplay()
{

}

void ScoreDisplay::OnEvent(Event * pEvent)
{
	AddScoreEvent* scoreEvent = static_cast<AddScoreEvent*>(pEvent);

	if (pEvent->GetEventType() == GameEventType_Score)
	{
		m_pCurrentScore += scoreEvent->GetScore();
	}
}

void ScoreDisplay::Update(float deltaTime)
{
	int score = m_pCurrentScore;

	m_pScoreDisplay[0] = score % 1000000 / 100000;
	m_pScoreDisplay[1] = score % 100000 / 10000;
	m_pScoreDisplay[2] = score % 10000 / 1000;
	m_pScoreDisplay[3] = score % 1000 / 100;
	m_pScoreDisplay[4] = score % 100 / 10;
	m_pScoreDisplay[5] = score % 10;
}

void ScoreDisplay::Draw(GameCamera * aCamera)
{
	if (m_pMesh != nullptr)
	{
		for (int i = 0; i < 6; i++)
		{
			m_pMesh->SetupAttributes(m_pMaterial);
			m_WorldPosition.SetIdentity();
			m_WorldPosition.CreateSRT(m_Scale, vec3(0, 0, 0), vec3(m_pFramePositions.at(i).x, m_pFramePositions.at(i).y, 0.0f));

			char tempstr[10];
			sprintf_s(tempstr, 10, "%d", m_pScoreDisplay[i]);

			m_pMaterial = m_pResources->GetMaterial(tempstr);

			m_pMesh->SetupUniformsUI(m_pMaterial, m_WorldPosition, 1.0f, aCamera);

			m_pMesh->Draw(m_pMaterial);
		}
	}
}

void ScoreDisplay::SetScore(int aScore)
{
	m_pCurrentScore = aScore;
}
