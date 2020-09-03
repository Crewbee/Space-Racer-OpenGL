#pragma once
class ResourceManager;
class SceneManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;
class TweenManager;

using namespace std;
class SplashScene : public Scene
{
protected:
	vector<UIObject*>m_pSplashScreenObjects;
	vec2 m_pWindowSize;
	mat4 m_pOrthoPos;

public:
	SplashScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~SplashScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	void AddUIObject(UIObject* aUIObject) { m_pSplashScreenObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void Reset() override;

private:
};