#pragma once
class Game;
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;

enum CountDown {
	Three,
	Two,
	One,
	GO
};


class HUDScene : public Scene
{
protected:
	vector<UIObject*>m_pHUDObjects;
	vec2 m_pWindowSize;
	mat4 m_pOrthoPos;
	CountDown m_pCountDown;
	float m_pCountDownTimer;

public:
	HUDScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~HUDScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	void AddUIObject(UIObject* aUIObject) { m_pHUDObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

	virtual void Reset() override;

private:
};