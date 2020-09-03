#pragma once
class Game;
class ResourceManager;
class SceneManager;
class TweenManager;
class AudioController;
class Scene;
class GameCamera;
class UIObject;
class Mesh;

class RenderScene : public Scene
{
protected:
	
	UIObject* m_pRenderObject;
	vec2 m_pWindowSize;
	mat4 m_pOrthoPos;

	Mesh* m_pRenderMesh;

public:
	RenderScene(Game* pGame, ResourceManager* pResources, AudioController* pAudioController, SceneManager* aSceneManager, TweenManager* pTweenManager);
	~RenderScene();

	void LoadContent();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;

	virtual void OnSurfaceChange(float percentage) override;

	virtual void OnScenePush() override;
	virtual void OnScenePop() override;

	virtual void Reset() override;

private:
};