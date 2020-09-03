#pragma once
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;
class UISliderObject;

using namespace std;
class AudioOptionsScene : public Scene
{

protected:
	vector<UIObject*>m_pAudioOptionsObjects;

	UISliderObject* m_pMusicSlider;
	UISliderObject* m_pFXSlider;

	mat4 m_pOrthoPos;
	vec2 m_pWindowSize;
	vec2 m_pAudioOptionsPos;
	vec2 m_pStartingAudioPos;
	vec4 m_pTargetScale;
public:
	AudioOptionsScene(Game* pGame, ResourceManager* pResources, AudioController* pAudio, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~AudioOptionsScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	virtual void OnSurfaceChange(float percentChange) override;

	void AddUIObject(UIObject* aUIObject) { m_pAudioOptionsObjects.push_back(aUIObject); }

	UIObject* GetUIObjectByName(string aName) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

    virtual void Reset() override;

private:

};