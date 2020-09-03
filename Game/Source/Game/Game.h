#pragma once
class ImGuiManager;
class ResourceManager;
class ResourceFactory;
class AudioController;
class SceneManager;
class TweenManager;
class FBO;
class SceneRenderer;

enum ResourceMode {
    JimmysJalopyMode,
    ImmersiveMode
};

class Game : public GameCore
{
protected:
    ImGuiManager* m_pImGuiManager;

    ResourceMode m_pMyMode;

    ResourceManager* m_pResources;
	ResourceFactory* m_pResourceFactory;

    FBO* m_pFrameBufferObject;

	SceneManager* m_pSceneManager;

    SceneRenderer* m_pSceneRenderer;

    TweenManager* m_pTweenManager;

	AudioController* m_pAudioController;

public:
    Game(Framework* pFramework);
    virtual ~Game();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height) override;
    virtual void LoadContent() override;

    virtual void OnEvent(Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

	void ResetFOV(float angle, vec2 aspectRatio, float Znear, float Zfar);

};

