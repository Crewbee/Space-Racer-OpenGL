#pragma once
#include "Game/ObjectPool.h"

class ResourceManager;
class Mesh;
class Scene;
class Event;
using namespace std;

class Planetoid : public GameObject
{
protected:

	vec3 m_ImpulseDirection;
	ObjectPool<GameObject*>* m_MyPool;
	ResourceManager* m_pResources;
	bool m_SetImpulse;
	int m_pHealth;

public:
	Planetoid(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, ResourceManager* pResources);
	~Planetoid();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual bool RemoveFromScene() override;

	void SetImpulseDirection(vec3 aDirection);
	void SetPool(ObjectPool<GameObject*>* aPool);

	bool GetIsSetImpulse() override { return m_SetImpulse; }
	void SetIsSetImpulse(bool setImpulse) override { m_SetImpulse = setImpulse; }

	virtual void ApplyDamage(int anAmount) override;
	virtual void Destroy() override;

	virtual void Reset();

private:
};



