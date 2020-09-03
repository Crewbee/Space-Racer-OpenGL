#pragma once
class Game;
class Scene;
class ResourceManager;
class AudioController;
class TweenManager;
class GameCamera;
class FBO;

enum Scene_Types {
	ST_SPLASH = 1,
	ST_MENU,
	ST_OPTIONS,
	ST_AUDIO,
	ST_PAUSE,
	ST_HUD,
	ST_GAMEPLAY,
	ST_GAMEOVER,
	ST_RENDER,
	ST_NUMSCENETYPES
};

enum Game_State {
	GS_INMENU,
	GS_BEGINPLAY,
	GS_RUNNING,
	GS_GAMEOVER,
	GS_WINGAME,
	GS_RESTART
};
using namespace std;

class SceneManager
{
protected:

	Game* m_pGame;

	ResourceManager* m_pResources;
	AudioController* m_pAudio;
    TweenManager* m_pTweenManager;
	FBO* m_pFrameBufferObject;

	vector<Scene*>m_pScenes;
	vector<Scene*>m_pActiveScenes;

	Scene_Types m_pCurrentScene;
	Game_State m_pCurrentGameState;
	vec2 m_pWindowSize;
	float m_pGameOverPushTimer;

	bool m_pPlayEndSound;

public:
	SceneManager(Game* pGame, ResourceManager* pResources, AudioController* pAudio, TweenManager* pTweenManager, FBO* pFrameBuffer);
	~SceneManager();

	void LoadContent();

	void OnEvent(Event* pEvent);

	void Update(float deltaTime);

	void Draw();

	void PopScene();
	void PushScene(Scene_Types aType);

	void ResetFOV(float angle, vec2 aspectRatio, float Znear, float Zfar);

	Scene* GetActiveScene() { return m_pActiveScenes.back(); }
	Scene* GetScene(Scene_Types aType);

	Scene_Types GetCurrentSceneType() {return m_pCurrentScene;}

	Game_State GetGameState() { return m_pCurrentGameState; }
	void SetGameState(Game_State aState) { m_pCurrentGameState = aState; }

	FBO* GetFrameBuffer() { return m_pFrameBufferObject; }

	float GetOnDeathTimer() { return m_pGameOverPushTimer; }

	void ResetGame();


private:
};