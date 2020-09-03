#pragma once
class GameObject;
class Scene;
class GameCamera;

class FollowCamera : public GameCamera
{
protected:
	vec3 m_pCameraOffset;
	GameObject* m_pTargetObject;

	bool m_MouseLocked;
	bool m_MouseMoved;
	bool m_ResetCursor;
	vec2 m_MouseChangeSinceLastFrame;
	vec2 m_LastMousePosition;
	vec2 m_MouseOffset;
	vec3 m_TargetRotation;
	vec3 m_LookAtOffset;
	vec3 m_LookAtTarget;


public:
	FollowCamera(Scene* aScene, std::string aName, vec3 pos, vec3 rot, vec3 scale, vec3 aTarget);
	~FollowCamera();

	virtual void OnEvent(Event* pEvent) override;

	void Update(float deltatime);

	virtual void SetObjectToFollow(GameObject* aTarget, vec3 anOffset) override;

	virtual vec3 SetTargetRotation();

	virtual void SetIsMouseLocked(bool isMouseLock) override { m_MouseLocked = isMouseLock; }

	virtual void ResetCursor(bool doResetCursor) override { m_ResetCursor = doResetCursor; }

	virtual void DetachCamera() override;

	virtual void Reset();

private:

};