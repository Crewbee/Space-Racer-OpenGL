#pragma once
#include "LightCasters.h"

class Material;
class Mesh;
class Scene;
class GameCamera;
using namespace std;
class Lamp : public GameObject
{
public:
	enum CasterType {
		CT_PointLight		= 0,
		CT_SpotLight		= 1,
		CT_NumCasterTypes	= 2
	};
protected:

	vector<pair<int, vec3>>m_RangeSettings = {	make_pair(7, vec3(1.0f, 0.7f, 1.8f)),			make_pair(13, vec3(1.0f, 0.35f,0.44f)),			make_pair(20, vec3(1.0f, 0.22f, 0.20f)),
												make_pair(32, vec3(1.0f, 0.14f, 0.07f)),		make_pair(50, vec3(1.0f, 0.09f, 0.032f)),		make_pair(65, vec3(1.0f, 0.07f, 0.017f)),
												make_pair(100, vec3(1.0f, 0.045f, 0.0075f)),	make_pair(160, vec3(1.0f, 0.027f, 0.0028f)),	make_pair(200, vec3(1.0f, 0.022f, 0.0019f)),
												make_pair(325, vec3(1.0f, 0.014f, 0.0007f)),	make_pair(600, vec3(1.0f, 0.007f, 0.0002f)),	make_pair(200, vec3(1.0f, 0.0014f, 0.000007f))};
	CasterType m_pCasterType;
	PointLight m_pMyPointLighting;
	SpotLight m_pMySpotLighting;

	bool m_TurnOffLamp;
	int m_MyRange;
	float m_MyLightStrength;

	vec3 m_LampOffset;

public:
	Lamp(Scene* pScene, std::string name, CasterType aType, vec3 pos, vec3 rot, vec3 scale, vec3 Offset, Mesh* pMesh, Material* pMaterial);
	~Lamp();

	virtual void Update(float deltatime);

	void SetUpLight(float strength, int range, vec3 lightDir, float inner, float outer);

	CasterType GetCasterType() { return m_pCasterType; }
	PointLight GetPointLighting() { return m_pMyPointLighting; }
	SpotLight GetSpotLighting() { return m_pMySpotLighting; }

	virtual void TurnOffLight(bool isTurnedOff) override { m_TurnOffLamp = isTurnedOff; }
	virtual bool GetIsLightOff() override { return m_TurnOffLamp; }

	void SetLampColor(vec3 aNewColor);
	vec3 GetLampColor();

	void SetRange(int aRange) { m_MyRange = aRange; }
	int GetRange() { return m_MyRange; }

	void SetStrength(float aLightStrength) { m_MyLightStrength = aLightStrength; }
	float GetStrength() { return m_MyLightStrength; }

	void SetLampOffset(vec3 anOffset) { m_LampOffset = anOffset; }
	vec3 GetLampOffset() { return m_LampOffset; }

private:

};
