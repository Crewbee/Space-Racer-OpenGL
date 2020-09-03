#pragma once
#include "GameObjects/GameObject.h"

class Material;
class Mesh;
class Scene;
class GameCamera;

using namespace std;

class UIObject : public GameObject
{
protected:
	vec2 m_pPositionOffset;
	vec2 m_pBaseScale;

	float m_pAlphaChannel;
	bool m_pDoesFade;
	bool m_pDoesFadeLoop;
	float m_pFadeFrequency;
	float m_pFadeTimer;

	bool m_pHoldAlpha;
	bool m_CanPlaySound;
	bool m_IsOpaque;

public:
	UIObject(Scene* pScene, std::string name, vec2 pOffset, vec2 scale, Mesh* pMesh, Material* pMaterial);
	~UIObject();

	virtual void OnEvent(Event* pEvent) override;

	virtual void Update(float deltaTime) override;

	virtual void Draw(GameCamera* aCamera) override;

	virtual void OnSurfaceChange(float percentChange) {};

	void SetPositionOffset(vec2 anOffset) { m_pPositionOffset = anOffset; }
	void SetPosition(vec2 aPosition) { m_Position = vec3(aPosition.x, aPosition.y, 0.0f); }
	void SetBaseScale(vec2 aScale);
	void SetScalerOffset(vec2 aScalar) { m_pBaseScale = aScalar; }

	vec2 GetOffset() { return m_pPositionOffset; }
	vec2 GetBaseScale() { return m_pBaseScale; }

	void SetAlphaChannel(float aValue);
	void SetDoesFade(float aFrequency, bool doesLoop);
	bool GetIsFading() { return m_pDoesFade; }

	float GetAlphaChannel() { return m_pAlphaChannel; }

	bool GetHoldAlpha() { return m_pHoldAlpha; }
	void SetHoldAlpha(bool holdAlpha) { m_pHoldAlpha = holdAlpha; }

	bool GetPlaysSound() { return m_CanPlaySound; }
	void SetPlaysSound(bool doesPlaySound) { m_CanPlaySound = doesPlaySound; }

	bool GetIsOpaque() { return m_IsOpaque; }
	void SetIsOpaque(bool notTransparent) { m_IsOpaque = notTransparent; }

	virtual void SetScore(int aScore) {};

private:

};