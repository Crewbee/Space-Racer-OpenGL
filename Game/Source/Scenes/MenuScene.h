#pragma once
class Game;
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;


class MenuScene : public Scene
{
protected:
	vector<UIObject*>m_pMenuObjects;
	vec2 m_pWindowSize;
	vec2 m_pMenuPos;
	vec2 m_pStartingMenuPos;
	mat4 m_pOrthoPos;

public:
	MenuScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~MenuScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	virtual void OnSurfaceChange(float percentChange) override;

	void AddUIObject(UIObject* aUIObject) { m_pMenuObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

	virtual void Reset() override;

private:
};