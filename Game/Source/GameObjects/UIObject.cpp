#include "GamePCH.h"

#include "UIObject.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Scenes/Scene.h"

#include "Events/SoundEvent.h"
#include "Game/Game.h"

UIObject::UIObject(Scene* pScene, std::string name, vec2 pOffset, vec2 scale, Mesh* pMesh, Material* pMaterial)
	:GameObject(pScene, name, UID_NULL, vec3(pOffset.x, pOffset.y, 0.0f), vec3(0, 0, 0), vec3(scale.x, scale.y, 0.0f), pMesh, pMaterial)
{
	m_pPositionOffset = pOffset;
	m_pBaseScale = scale;
	m_pAlphaChannel = 1.0f;
	m_pDoesFade = false;
	m_pDoesFadeLoop = false;
	m_pFadeFrequency = 0.0f;
	m_pFadeTimer = 0.0f;
	m_pHoldAlpha = false;
	m_CanPlaySound = false;
	m_IsOpaque = false;
}

UIObject::~UIObject()
{
}

void UIObject::OnEvent(Event * pEvent)
{
}

void UIObject::Update(float deltaTime)
{
	if (m_pDoesFade)
	{
		m_pFadeTimer += deltaTime;
		float FadeIncrement = (m_pFadeFrequency * m_pFadeTimer) / 180 * PI;

		if (FadeIncrement > PI)
		{
			FadeIncrement = 0.0f;
			m_pFadeTimer = 0.0f;
			if (!m_pDoesFadeLoop)
			{
				m_pDoesFade = false;
				m_pAlphaChannel = 0.0f;
			}
		}

		m_pAlphaChannel = sinf(FadeIncrement);
	}
}

void UIObject::Draw(GameCamera * aCamera)
{
	if (m_pMesh != nullptr)
	{
		m_pMesh->SetupAttributes(m_pMaterial);

		m_WorldPosition.SetIdentity();
		m_WorldPosition.CreateSRT(m_Scale, vec3(0, 0, 0), m_Position);

		m_pMesh->SetupUniformsUI(m_pMaterial, m_WorldPosition, m_pAlphaChannel, aCamera);

		m_pMesh->Draw(m_pMaterial);
	}
}

void UIObject::SetBaseScale(vec2 aScale)
{
	m_Scale = vec3(m_pBaseScale.x, m_pBaseScale.y, 0.0f);
}

void UIObject::SetAlphaChannel(float aValue)
{
	if (m_CanPlaySound && m_IsOpaque)
	{
		if (aValue != m_pAlphaChannel && aValue != 1.0f)
		{
			SoundEvent* psound = new SoundEvent(Sounds::MouseOver, PlayPriority::CanPlay);
			m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);
		}
	}
	if (!m_pHoldAlpha)
	{
		m_pAlphaChannel = aValue;
	}
}

void UIObject::SetDoesFade(float aFrequency, bool doesLoop)
{
	m_pDoesFade = true;
	m_pFadeFrequency = aFrequency;
	m_pDoesFadeLoop = doesLoop;
	m_pAlphaChannel = 0.0f;
}
