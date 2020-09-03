#pragma once
#include "GameObjects/LightCasters.h"
#include "Helpers/SceneManager.h"

class Game;
class GameObject;
class UIObject;
class Player;
class Lamp;
class GameCamera;
class ResourceManager;
class BulletManager;
class BulletDebugDraw;
class AudioController;
class SceneManager;
class TweenManager;
class FBO;
class SceneRenderer;
class DebrisSpawner;
class RingSpawner;

using namespace std;

class Scene
{
private:

protected:
	Game* m_pGame;
	ResourceManager* m_pResources;
	BulletManager* m_pBulletManager;;
	BulletDebugDraw* m_pDebugDrawer;
	AudioController* m_pAudio;
	SceneManager* m_pSceneManager;
    TweenManager* m_pTweenManager;
	DebrisSpawner* m_pDebrisSpawner;
	RingSpawner* m_pRingSpawner;

	Scene_Types m_Type;

	std::vector<GameObject*> m_pGameObjects;
	GameCamera* m_pGameCamera;

	Player* m_pPlayer;

	bool m_pHasLighting;
	bool m_pDoDebugDraw;

	vector<Lamp*>m_pPointLights;
	vector<Lamp*>m_pSpotLights;

	vector<PointLight>m_PointLightProperties;
	vector<SpotLight>m_SpotLightProperties;

public:
	Scene(Game* pGame, ResourceManager* pResources, SceneManager* pSceneManager, AudioController* pAudio, TweenManager* pTweenManager);
	virtual ~Scene();

	void Cleanup();

	virtual void LoadContent();

	virtual void Reset();

	virtual void OnEvent(Event* pEvent);
	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void OnSurfaceChange(float percentChange) {};

	//Setters
	virtual void SetHasLighting(bool IsLit) { m_pHasLighting = IsLit; }
	virtual void SetUpPointLights(Lamp* aPointLight, int anIndex);
	virtual void SetUpSpotLights(Lamp* aSpotLight, int anIndex);
	virtual void SetSceneType(Scene_Types aType) { m_Type = aType; }

	virtual void OnScenePush();
	virtual void OnScenePop();

	//Getters
	GameCamera* GetCamera() { return m_pGameCamera; }
	Game* GetGame() { return m_pGame; }
	ResourceManager* GetResourceManager() { return m_pResources; }
	GameObject* GetGameObjectByName(std::string name);
	DebrisSpawner* GetDebrisSpawner() { return m_pDebrisSpawner; }
	SceneManager* GetSceneManager() { return m_pSceneManager; }
	virtual BulletManager* GetBulletManager() { return m_pBulletManager; }
	virtual Player* GetPlayer() { return m_pPlayer; }
	virtual GameObject* GetObjectAtLocation(vec3 aPosition);
	virtual SpotLight* GetSpotLights();
	virtual PointLight* GetPointLights();
	virtual bool GetHasLighting() { return m_pHasLighting; }
	virtual Scene_Types GetType() { return m_Type; }
	virtual TweenManager* GetTweenManager() { return m_pTweenManager; }
	virtual UIObject* GetUIObjectByName(string aName) { return nullptr; }


	//Add and Remove GameObjects
	void AddGameObject(GameObject* pObject);
	bool RemoveGameObject(GameObject* pObject);
	bool IsGameObjectInScene(GameObject* pObject);
};
