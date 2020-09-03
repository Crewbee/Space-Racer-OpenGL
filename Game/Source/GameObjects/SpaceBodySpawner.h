#pragma once
#include "Game/ObjectPool.h"

class ResourceManager;
class Scene;

class SpaceBodySpawner : public GameObject
{
protected:

	ObjectPool<GameObject*> m_Asteroids;
	ObjectPool<GameObject*> m_Planetoids;
	ResourceManager* m_pResources;
	float m_Offset;

	int m_AsteroidCount;
	int m_PlanetoidCount;

	int m_DistAsteroidSpawn;

	int m_DistPlanetoidSpawn;

	int m_CheckZChanged;

	float PlanetoidSpawnTimer;
	float AsteroidSpawnTimer;

	float m_pNextAsteroidSpawnPos;
	float m_pNextPlanetoidSpawnPos;

public:
	SpaceBodySpawner(Scene* pScene, ResourceManager* pResources, vec3 pos);
	~SpaceBodySpawner();

	virtual void Update(float deltaTime) override;

	void SpawnAsteroids();
	void SpawnPlanetoids();

	void LoadContent();

	void Reset();

private:

};

