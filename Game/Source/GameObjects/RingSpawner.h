#pragma once
#include "Game/ObjectPool.h"

class ResourceManager;
class Scene;
class Mesh;
class Material;
class GameCamera;
class DebrisObject;
class GameObject;

using namespace std;

class RingSpawner
{
protected:
	const int MaxDebris = 30;

	ObjectPool<GameObject*>m_RingPool;

	vector<GameObject*>m_pActiveRings;

	GameObject* m_pFinishRing;

	ResourceManager* m_pResources;
	Scene* m_pMyScene;

	float m_pPlayerZ;
	float m_pRingSpacing;
	float m_pStartingSpawnTimer;

public:
	RingSpawner(Scene* pScene, ResourceManager* pResources);
	~RingSpawner();

	void OnEvent(Event* pEvent);

	void Update(float deltaTime);

	void Draw(GameCamera* aCamera);

	void SpawnRing(vec3 aPosition);
	bool RemoveRing(GameObject* aDebris);
	bool RingIsActive(GameObject* aDebris);

	void Reset();

private:
};