#pragma once
#include "GameObjects/GameObject.h"
class Scene;

class GameCamera : public GameObject
{
protected:
	float m_pCamNear;
	float m_pCamDist;
	float m_pCamFar;
	vec2 m_WindowSize;

    mat4 m_pViewMatrix;
    mat4 m_pProjMatrix;
    vec3 m_pLookAtPosition;
	vec3 m_pUpVector;
	vec3 m_pStartTarget;

	//OrthoCamera
	vec4 m_pOrthoVector;

public:
	GameCamera(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, vec3 aTarget);
    virtual ~GameCamera();

    virtual void Update(float deltaTime) override;

    mat4* GetViewMatrix() { return &m_pViewMatrix; }
    mat4* GetProjMatrix() { return &m_pProjMatrix; }
	vec2 GetWindowSize() { return m_WindowSize; }
    void SetLookAtPosition(vec3 pos) { m_pLookAtPosition = pos; }

	virtual void SetObjectToFollow(GameObject* aTarget, vec3 anOffset) {};
	virtual void SetupViewLookAt(vec3 aPosition, vec3 anUpVector, vec3 aTarget);
	virtual void OnSurfaceChange(float angle, vec2 aspectRatio, float Znear, float Zfar);

	//OrthoCamera Overrides
	virtual vec4 GetOrthoVector() { return m_pOrthoVector; }
	virtual void SetOrthoProj(vec4 anOrthoVector) {};

	//Follow Camera overrides
	virtual void SetIsMouseLocked(bool isMouseLocked) {};
	virtual void DetachCamera() {};
	virtual void ResetCursor(bool doResetCursor) {};
};
