#pragma once
#include "GameObjects/GameObject.h"

class GameObject;
class Scene;
class GameCamera;

class OrthoCamera : public GameCamera
{
protected:

public:
	OrthoCamera(Scene* pScene, std::string name, vec3 pos);
	~OrthoCamera();

	virtual void OnEvent(Event* pEvent) override;

	virtual void Update(float deltaTime) override;

	virtual void OnSurfaceChange(float angle, vec2 aspectRatio, float Znear, float Zfar) override;

	virtual vec4 GetOrthoVector() override;
	virtual void SetOrthoProj(vec4 anOrthoVector) override;

private:

};