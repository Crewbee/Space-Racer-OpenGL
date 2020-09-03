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

class DebrisSpawner
{
protected:
	const int MaxDebris = 500;

	ObjectPool<GameObject*>m_DebrisPool;

	vector<GameObject*>m_pActiveDebris;

	Mesh* m_pDebrisMesh;
	Material* m_pDebrisMat;

	ResourceManager* m_pResources;
	Scene* m_pMyScene;

	int m_pNumActiveParticles;
	float m_pCamZPos;

public:
	DebrisSpawner(Scene* pScene, Mesh* pMesh, Material* pMaterial, ResourceManager* pResources);
	~DebrisSpawner();

	void OnEvent(Event* pEvent);
	void Update(float deltaTime);

	void Draw(GameCamera* aCamera);

	//************************************
	// Will spawn 40 debris objects at the position at a tenth of the radius with a random velocity based on radius size
	//************************************
	void SpawnDebris(vec3 aPosition, float Radius);
	//************************************
	// Removes a debris object from the scene
	//************************************
	bool RemoveDebris(GameObject* aDebris);
	//************************************
	// Returns true if the debris object is active
	//************************************
	bool DebrisIsActive(GameObject* aDebris);
	//************************************
	// Return all of the debris to the pool and clear the active debris vector
	//************************************
	virtual void Reset();

private:
};