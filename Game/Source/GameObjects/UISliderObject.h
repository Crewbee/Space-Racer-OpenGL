#pragma once
#include "XAudio2/SoundPlayer.h"

class ResourceManager;
class UIObject;
class Scene;
class Mesh;
class Material;
class Scene;
class GameCamera;

using namespace std;

class UISliderObject : public UIObject
{
protected:

	ResourceManager* m_pResources;
	UIObject* m_pSliderNub;
	mat4 m_pOrtho;
	float m_pSliderValue;
	vec2 m_pMousePosition;
	vec2 m_pNubMinMax;
	PlayerType m_pMyPlayer;
	bool m_pNubTrackMouse;


public:
	UISliderObject(Scene* pScene, std::string name, vec2 pOffset, vec2 scale, Mesh* pMesh, Material* pMaterial, ResourceManager* pResources, mat4 Ortho, PlayerType pSoundPlayer);
	~UISliderObject();

	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime);

	virtual void Draw(GameCamera* aCamera) override;

	void SetSiderValue(float aValue);

	float GetSliderValue();

	PlayerType GetMyPlayer() { return m_pMyPlayer; }

	virtual void OnSurfaceChange(float percentChange) override;

private:

};