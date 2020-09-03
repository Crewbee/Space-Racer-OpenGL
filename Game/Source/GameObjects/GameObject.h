#pragma once
#include "Game/ObjectPool.h"

class Material;
class Scene;
class Mesh;
class btRigidBody;
class GameCamera;

	enum CollisionID {
		UID_NULL			= -1,
		UID_PLAYER			= 0,
		UID_MOON			= 1,
		UID_ASTEROID		= 2,
		UID_UNIVERSE		= 4,
		UID_NumCollisionID	= 8
	};
class GameObject
{

protected:
    std::string m_Name;

	CollisionID m_pCollisionID;

    Scene* m_pScene;

    Mesh* m_pMesh;
    Material* m_pMaterial;

    vec3 m_InitialPosition;
    vec3 m_InitialRotation;

    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;

	mat4 m_WorldPosition;

    vec2 m_UVScale;
    vec2 m_UVOffset;

	//Rotator
	vec3 m_pRotatorAxis;
	float m_pRotatorSpeed;

    float m_pTweenModifier;

	btRigidBody* m_pBody;
	bool m_pSetImpluse;

	//Is currently being updated
	bool m_pIsActive;

	//SpaceBodies
	vec3 m_pImpulseDirection;
	float m_pLifeTime;

	//Is Tween Completed
	bool m_pIsTweening;

public:
    GameObject(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~GameObject();

    virtual void Reset();

    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltaTime);
    virtual void Draw(GameCamera* pCamera);

	virtual btQuaternion CalculateQuaternionRotation(vec3 aRotation);
	virtual vec3 GetEulerRotation(vec3 aRotation);
	virtual void CreateBody(btCollisionShape* aShape, btScalar Mass, bool isDynamic);
	//************************************
	// Creates a Box Collision Body
	//************************************
	virtual void CreateBoxBody(vec3 aSize, btScalar aMass, bool isDynamic);
	//************************************
	// Creates a Collision Body using the Object Vertex Positions
	//************************************
	virtual void CreateHullBody(btScalar aMass, bool isDynamic);
	//************************************
	// Creates a Spherical Collision Body
	//************************************
	virtual void CreateSphereBody(btScalar aRadius, float aProportion, btScalar aMass, bool isDynamic);

    // Getters.
    std::string GetName() { return m_Name; }
    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
	//************************************
	// Returns the largest Radius
	//************************************
	float GetMeshRadius();
	//************************************
	// Returns a Vector3 of the Max XYZ
	//************************************
	vec3 GetMeshSize();
	vec3 GetDirectionVector();
	btRigidBody* GetBody() { return m_pBody; }
	CollisionID GetCollisionID() { return m_pCollisionID; }
	virtual float* GetTweenModifier() { return &m_pTweenModifier; }
    vec3* GetTweenXYZCoords() { return &m_Position; }
    vec3* GetTweenScaleCoords() { return &m_Scale; }
	bool GetIsActive() { return m_pIsActive; }
	bool GetIsTweening() { return m_pIsTweening; }


    // Setters.
    void SetPosition(vec3 pos);
    void SetRotation(vec3 rot);
	void SetScale(vec3 scale);
    void SetUVScaleAndOffset(vec2 uvScale, vec2 uvOffset);
	virtual void SetControllerAxis(vec2 aControllerAxis) {};
	void AddRotation(vec3 anAxis, float aSpeed);
	void SetIsActive(bool isActive) { m_pIsActive = isActive; }
	void SetIsTweening(bool isTweening) { m_pIsTweening = isTweening; }

	//virtuals for space bodies
	virtual bool GetIsSetImpulse() { return m_pSetImpluse; }
	virtual void SetIsSetImpulse(bool setImpulse) { m_pSetImpluse = setImpulse; }
	virtual void SetImpulseDirection(vec3 aVelocity) { m_pImpulseDirection = aVelocity; }
	virtual void Destroy() {};
	virtual void ApplyDamage() {};
	virtual void SetPool(ObjectPool<GameObject*>* aPool) {};
	virtual float GetLifeTime() { return m_pLifeTime; }
	virtual void SetLifeTime(float aLifeTime) { m_pLifeTime = aLifeTime; }
	virtual void ApplyDamage(int anAmount) {};

	//virtuals for player
	virtual void OnDeath() {};
	virtual void OnPlayerWin() {};

    // Other helpers.
    virtual void ReturnToPool();	
    virtual bool RemoveFromScene();
	virtual bool GetIsLightOff() { return false; }
	virtual void TurnOffLight(bool isLightOff) {};

};
