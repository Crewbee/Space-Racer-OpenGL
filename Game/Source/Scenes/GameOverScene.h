#pragma once
class Game;
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;

class GameOverScene : public Scene
{
protected:
	vector<UIObject*>m_pGameOverObjects;
	vec2 m_pWindowSize;
	mat4 m_pOrthoPos;

public:
	GameOverScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~GameOverScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	void AddUIObject(UIObject* aUIObject) { m_pGameOverObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

    virtual void Reset() override;

private:
};