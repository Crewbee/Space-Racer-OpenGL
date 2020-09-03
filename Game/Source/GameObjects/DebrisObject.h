#pragma once
#include "Game/ObjectPool.h"
#include "GameObjects/GameObject.h"
class Material;
class Scene;
class Mesh;
class GameCamera;
class DebrisSpawner;

class DebrisObject : public GameObject
{
protected:

	DebrisSpawner* m_pMySpawner;
	vec3 m_ImpulseDirection;
	ObjectPool<GameObject*>* m_MyPool;
	bool m_SetImpluse;

public:
	DebrisObject(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial, DebrisSpawner* pDebrisSpawner);
	virtual ~DebrisObject();

	virtual void Update(float deltaTime) override;

	virtual void Draw(GameCamera* aCamera) override;

	//************************************
	// Called when the scene or spawner removed the object from the scene
	//************************************
	virtual bool RemoveFromScene() override;

	//************************************
	// Set the rotation / direction
	//************************************
	virtual void SetImpulseDirection(vec3 aDirection) override;
	//************************************
	// Setup the reference to an Object Pool
	//************************************
	virtual void SetPool(ObjectPool<GameObject*>* aPool) override;

	//************************************
	// Get the current life time of the object
	//************************************
	virtual float GetLifeTime() override { return m_pLifeTime; }
	//************************************
	// Set the life time to a specified value
	//************************************
	virtual void SetLifeTime(float aLifeTime) override { m_pLifeTime = aLifeTime; }

	//************************************
	// Get whether the object will have its position/rotation incremented 
	//************************************
	bool GetIsSetImpulse() override { return m_SetImpluse; }
	//************************************
	// Set whether the object will have its position/rotation incremented 
	//************************************
	void SetIsSetImpulse(bool setImpulse) override { m_SetImpluse = setImpulse; }

	//************************************
	// Remove the object from the scene
	//************************************
	virtual void Destroy() override;

};