#pragma once
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;

using namespace std;
class OptionScene : public Scene
{

protected:
	vector<UIObject*>m_pOptionObjects;
	mat4 m_pOrthoPos;
	vec2 m_pWindowSize;
	vec2 m_pOptionsPos;
	vec2 m_pStartingOptionPos;

public:
	OptionScene(Game* pGame, ResourceManager* pResources, AudioController* pAudio, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~OptionScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	virtual void OnSurfaceChange(float percentChange) override;

	void AddUIObject(UIObject* aUIObject) { m_pOptionObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

	virtual void Reset() override;

private:

};