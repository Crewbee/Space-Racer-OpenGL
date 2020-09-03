#pragma once
class ResourceManager;
class UIObject;
class Scene;
class Mesh;
class Material;
class Scene;
class GameCamera;

using namespace std;

class ScoreDisplay : public UIObject
{
protected:

	int m_pScoreDisplay[6];
	int m_pCurrentScore;

	vector<vec2>m_pFramePositions;

	ResourceManager* m_pResources;

	mat4 m_pOrtho;

public:
	ScoreDisplay(Scene* pScene, std::string name, vec2 pOffset, vec2 scale, Mesh* pMesh, Material* pMaterial, ResourceManager* pResources, mat4 Ortho);
	~ScoreDisplay();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime);

	virtual void Draw(GameCamera* aCamera) override;

	virtual void SetScore(int aScore) override;

private:

};