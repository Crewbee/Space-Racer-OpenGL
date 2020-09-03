#pragma once
#include "Scenes/Scene.h"
#include "Game/ObjectPool.h"

class Game;
class GameObject;
class Player;
class ResourceManager;
class TweenManager;
class BulletManager;
class SpaceBodySpawner;
class ParticleEmitter;

class LightingScene : public Scene
{
	Player* m_pSpaceShip;
	SpaceBodySpawner* m_pSpawner;
	bool m_DoRenderToTexture;
	bool m_PlayerDead;

public:
	LightingScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	virtual ~LightingScene();

	virtual void LoadContent() override;

	virtual void OnEvent(Event* pEvent) override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	virtual Player* GetPlayer() override { return m_pSpaceShip; }

	void RemovePlayer();

	virtual void Reset() override;

private:

};
