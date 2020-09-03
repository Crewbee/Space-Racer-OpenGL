#include "GamePCH.h"

#include "GameObjects/UIObject.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"
#include "UISliderObject.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Scenes/Scene.h"

#include "Events/AddScoreEvent.h"
#include "Events/GameEventTypes.h"

UISliderObject::UISliderObject(Scene * pScene, std::string name, vec2 pOffset, vec2 scale, Mesh * pMesh, Material * pMaterial, ResourceManager * pResources, mat4 Ortho, PlayerType pSoundPlayer)
	:UIObject(pScene, name, pOffset, scale, pMesh, pMaterial)
{
	m_pResources = pResources;
	m_pOrtho = Ortho;
	m_pSliderNub = new UIObject(pScene, name + "nub", vec2(0.0f, -0.07f) + pOffset, vec2(0.006f, 0.14f), m_pResources->GetMesh("UIRectangleMesh"), m_pResources->GetMaterial("SliderNubMaterial"));
	vec4 SliderNubPos = m_pOrtho.GetInverse() * vec4(m_pSliderNub->GetOffset().x, m_pSliderNub->GetOffset().y, 0.0f, 0.0f);
	m_pSliderNub->SetPosition(vec2(SliderNubPos.x, SliderNubPos.y));
	m_pSliderNub->SetScale(vec3(m_pSliderNub->GetBaseScale().x * (m_pScene->GetCamera()->GetWindowSize().x / m_pScene->GetCamera()->GetWindowSize().y), m_pSliderNub->GetBaseScale().y, 0.0f));
	m_pSliderNub->SetPlaysSound(false);
	m_pSliderValue = 0.5f;

	m_pMousePosition.Set(0.0f, 0.0f);

	m_pNubTrackMouse = false;

	m_pMyPlayer = pSoundPlayer;

	m_pNubMinMax.Set(-0.7f, 0.7f);
}

UISliderObject::~UISliderObject()
{
	delete m_pSliderNub;
}

void UISliderObject::OnEvent(Event * pEvent)
{
	InputEvent* pInput = static_cast<InputEvent*>(pEvent);
	if (pInput->GetInputDeviceType() == InputDeviceType_Mouse)
	{
		vec2 MousePos = pInput->GetPosition();
		MousePos = MousePos / m_pScene->GetCamera()->GetWindowSize();
		MousePos = MousePos * 2.0f - 1.0f;
		MousePos.y *= -1.0f;

		vec4 ConvertToClip = m_pOrtho.GetInverse() * vec4(MousePos.x, MousePos.y, 0.0f, 0.0f);

		m_pMousePosition = vec2(ConvertToClip.x, ConvertToClip.y);

		vec2 ButtonPos = vec2(m_pSliderNub->GetPosition().x, m_pSliderNub->GetPosition().y);
		vec2 ButtonSize = vec2(m_pSliderNub->GetMeshSize().x, m_pSliderNub->GetMeshSize().y);
		{
			if (m_pMousePosition.x > (ButtonPos.x - (ButtonSize.x / 2)) && m_pMousePosition.x < (ButtonPos.x + (ButtonSize.x / 2)) && m_pMousePosition.y >(ButtonPos.y - (ButtonSize.y / 2)) && m_pMousePosition.y < (ButtonPos.y + (ButtonSize.y / 2)))
			{
				if (pInput->GetInputState() == InputState_Pressed && pInput->GetID() == 0)
				{
					m_pNubTrackMouse = true;
				}
			}
		}
		if (m_pNubTrackMouse == true)
		{
			if (pInput->GetInputState() == InputState_Released && pInput->GetID() == 0)
			{
				m_pNubTrackMouse = false;
			}
		}
	}
}

void UISliderObject::Update(float deltaTime)
{

	if (m_pNubTrackMouse)
	{
		m_pSliderNub->SetPosition(vec2(m_pMousePosition.x, m_pSliderNub->GetPosition().y));

		float sliderValue = ((m_pSliderNub->GetPosition().x - m_pPositionOffset.x) + m_pNubMinMax.y) / (m_pNubMinMax.y * 2.0f);

		m_pSliderValue = sliderValue;
	}
	if (m_pSliderNub->GetPosition().x < m_pNubMinMax.x + m_pPositionOffset.x)
	{
		m_pSliderNub->SetPosition(vec2(m_pNubMinMax.x + m_pPositionOffset.x, m_pSliderNub->GetPosition().y));
	}
	if (m_pSliderNub->GetPosition().x > m_pNubMinMax.y + m_pPositionOffset.x)
	{
		m_pSliderNub->SetPosition(vec2(m_pNubMinMax.y + m_pPositionOffset.x, m_pSliderNub->GetPosition().y));
	}
}

void UISliderObject::Draw(GameCamera * aCamera)
{
	UIObject::Draw(aCamera);

	m_pSliderNub->Draw(aCamera);
}

void UISliderObject::SetSiderValue(float aValue)
{
	m_pSliderValue = aValue;
}

float UISliderObject::GetSliderValue()
{
	return m_pSliderValue;
}

void UISliderObject::OnSurfaceChange(float percentChange)
{
	m_pOrtho = *m_pScene->GetCamera()->GetProjMatrix();
	vec4 RelativePos = m_pOrtho.GetInverse() * vec4(m_pSliderNub->GetPosition().x, m_pSliderNub->GetPosition().y, 0.0f, 0.0f);
	m_pSliderNub->SetPosition(vec2(RelativePos.x, RelativePos.y));
	m_pSliderNub->SetIsOpaque(true);

	m_pNubMinMax.Set((m_Position.x - (GetMeshSize().x / 2.0f)) - m_pPositionOffset.x + 0.1f, (m_Position.x + (GetMeshSize().x / 2)) + m_pPositionOffset.x - 0.1f);
}
