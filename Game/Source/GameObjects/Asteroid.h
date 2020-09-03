#pragma once
#include "Game/ObjectPool.h"

class Material;
class Scene;
class Mesh;

class Asteroid : public GameObject
{ 
protected:
	
	vec3 m_ImpulseDirection;
	ObjectPool<GameObject*>* m_MyPool;
	bool m_SetImpluse;
	int m_pHealth;

public:
	Asteroid(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
	virtual ~Asteroid();

	virtual void Update(float deltaTime) override;

	//************************************
	// Called when the scene or spawner removed the object from the scene
	//************************************
	virtual bool RemoveFromScene() override;

	//************************************
	// Set the rotation / direction
	//************************************
	void SetImpulseDirection(vec3 aDirection);
	//************************************
	// Setup the reference to an Object Pool
	//************************************
	void SetPool(ObjectPool<GameObject*>* aPool);

	//************************************
	// Get whether the object will have its position/rotation incremented 
	//************************************
	bool GetIsSetImpulse() override { return m_SetImpluse; }
	//************************************
	// 	// Set whether the object will have its position/rotation incremented 
	//************************************
	void SetIsSetImpulse(bool setImpulse) override { m_SetImpluse = setImpulse; }

	//************************************
	// Subtract an amount from the objects health
	//************************************
	virtual void ApplyDamage(int anAmount) override;
	//************************************
	// Removes the object from the scene
	// will be called automatically if the object has no more health
	//************************************
	virtual void Destroy() override;

	//************************************
	// Resets the object
	// If the object is a pooled object it will be returned to the pool
	//************************************
	virtual void Reset();

};