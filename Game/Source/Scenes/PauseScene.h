#pragma once
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class Game;
class GameCamera;
class UIObject;

class PauseScene : public Scene
{
protected:

	vector<UIObject*>m_pPauseObjects;
	vec2 m_pWindowSize;
	mat4 m_pOrthoPos;

public:
	PauseScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~PauseScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	void AddUIObject(UIObject* aUIObject) { m_pPauseObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

	virtual void Reset() override;

private:
};