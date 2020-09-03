#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "Lamp.h"
#include "GameObjects/Player.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

#include "Helpers/ResourceManager.h"
#include "Scenes/Scene.h"

Lamp::Lamp(Scene* pScene, std::string name, CasterType aType, vec3 pos, vec3 rot, vec3 scale, vec3 Offset, Mesh* pMesh, Material* pMaterial)
	: GameObject(pScene, name, UID_NULL, pos, rot, scale, pMesh, pMaterial)
{
	m_pCasterType = aType;
	m_TurnOffLamp = false;
	m_MyRange = 0;
	m_MyLightStrength = 0.0f;
	m_LampOffset = Offset;
}
Lamp::~Lamp()
{

}
void Lamp::Update(float delatime)
{
	GameObject::Update(delatime);

	m_Position = m_pScene->GetPlayer()->GetPosition() + m_LampOffset;

	if (m_pCasterType == CT_PointLight)
	{
		m_pMyPointLighting.Position = GetPosition();
		MyColor col = m_pMaterial->GetColor();
		if (m_TurnOffLamp)
		{
			m_pMyPointLighting.LightColor = vec3(0, 0, 0);
		}
		else
		{
			vec3 LightCol = vec3(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f);
			m_pMyPointLighting.LightColor = LightCol;
		}
		m_pMyPointLighting.linear = m_RangeSettings.at(m_MyRange).second.y;
		m_pMyPointLighting.quadratic = m_RangeSettings.at(m_MyRange).second.z;
		m_pMyPointLighting.Strength = m_MyLightStrength;
	}
	else if (m_pCasterType == CT_SpotLight)
	{
		m_pMySpotLighting.Position = GetPosition();
		MyColor col = m_pMaterial->GetColor();
		if (m_TurnOffLamp)
		{
			m_pMySpotLighting.LightColor = vec3(0, 0, 0);
			col.a = 0;
			m_pMaterial->SetColor(col);
		}
		else
		{
			col.a = 255;
			m_pMaterial->SetColor(col);
			vec3 LightCol = vec3(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f);
			m_pMySpotLighting.LightColor = LightCol;
		}
		vec3 SpotLightDir = m_pScene->GetPlayer()->GetRotation();
		SetRotation(SpotLightDir);

		SpotLightDir.Normalize();

		SpotLightDir = vec3(-SpotLightDir.y, SpotLightDir.x, cos(SpotLightDir.z));

		m_pMySpotLighting.Direction = SpotLightDir;

		m_pMySpotLighting.linear = m_RangeSettings.at(m_MyRange).second.y;
		m_pMySpotLighting.quadratic = m_RangeSettings.at(m_MyRange).second.z;
		m_pMySpotLighting.Strength = m_MyLightStrength;
	}
}

void Lamp::SetUpLight(float strength, int range, vec3 lightDir, float inner, float outer)
{
	m_MyRange = range;

	if (m_pCasterType == CT_PointLight)
	{
		m_pMyPointLighting.Position = GetPosition() + m_LampOffset;
		MyColor col = m_pMaterial->GetColor();
		vec3 LightCol = vec3(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f);
		m_pMyPointLighting.LightColor = LightCol;

		m_pMyPointLighting.Strength = strength;
		m_pMyPointLighting.Distance = (int)m_RangeSettings.at(range).first;

		m_pMyPointLighting.linear = m_RangeSettings.at(range).second.y;
		m_pMyPointLighting.quadratic = m_RangeSettings.at(range).second.z;
	}
	else if (m_pCasterType == CT_SpotLight)
	{
		m_pMySpotLighting.Position = GetPosition() + m_LampOffset;
		MyColor col = m_pMaterial->GetColor();
		vec3 LightCol = vec3(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f);
		m_pMySpotLighting.LightColor = LightCol;

		m_pMySpotLighting.Direction = lightDir;

		m_pMySpotLighting.innerCutOff = cos(inner / 180.0f * PI);
		m_pMySpotLighting.outerCutOff = cos(outer / 180.0f * PI);

		m_pMySpotLighting.Strength = strength;
		m_pMySpotLighting.Distance = (int)m_RangeSettings.at(range).first;

		m_pMySpotLighting.linear = m_RangeSettings.at(range).second.y;
		m_pMySpotLighting.quadratic = m_RangeSettings.at(range).second.z;
	}
}
void Lamp::SetLampColor(vec3 aNewColor)
{
	MyColor NewColor = MyColor((char)(aNewColor.x * 255), (char)(aNewColor.y * 255), char(aNewColor.z * 255), 255);

	m_pMaterial->SetColor(NewColor);

}
vec3 Lamp::GetLampColor()
{
	MyColor currentColor = m_pMaterial->GetColor();

	return vec3(currentColor.r / 255.0f, currentColor.g / 255.0f, currentColor.b / 255.0f);
}