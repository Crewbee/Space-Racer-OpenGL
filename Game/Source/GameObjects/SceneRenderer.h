#pragma once
#include "GameObjects/UIObject.h"

class Mesh;
class Material;
class FBO;
class Scene;
class ResourceManager;

class SceneRenderer : public UIObject
{
protected:
	Scene* m_pScene;
	Material* m_pMaterial;
	Mesh* m_pMesh;
	FBO* m_pFrameBuffer;

public:
	SceneRenderer(Scene* pScene, vec2 pOffset, Mesh* pMesh, Material* pMaterial);
	~SceneRenderer();

	void Draw(GameCamera* aCamera);

	void LoadFrame(GLuint aFrame);

private:
};