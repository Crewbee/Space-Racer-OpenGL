#pragma once
#include "Game/ObjectPool.h"
#include "GameObjects/GameObject.h"
class Material;
class Scene;
class Mesh;
class GameCamera;
class RingSpawner;

class SpaceRing : public GameObject
{
protected:

	RingSpawner* m_pMySpawner;
	vec3 m_ImpulseDirection;
	ObjectPool<GameObject*>* m_MyPool;

public:
	SpaceRing(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial, RingSpawner* pDebrisSpawner);
	virtual ~SpaceRing();

	virtual void Update(float deltaTime) override;

	virtual void Draw(GameCamera* aCamera) override;

	virtual bool RemoveFromScene() override;

	virtual void SetPool(ObjectPool<GameObject*>* aPool) override;

	virtual void Destroy() override;

};
