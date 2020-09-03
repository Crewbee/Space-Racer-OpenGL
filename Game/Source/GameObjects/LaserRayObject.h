#pragma once
class Material;
class Mesh;
class Scene;
class GameCamera;
class Player;
using namespace std;

class LaserRayObject : public GameObject
{
protected:

	bool m_pDrawLaser;
	vec3 m_pMuzzle;
	vec2 m_pLaserWidth;
	Player* m_pMyPlayer;

public:
	LaserRayObject(Scene* pScene, string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial, Player* pPlayer);
	~LaserRayObject();


	virtual void OnEvent(Event* pEvent);

	virtual void Update(float deltaTime);

	virtual void Draw(GameCamera* aCamera);

	void SetMuzzleLocation(vec3 aMuzzleLocation) { m_pMuzzle = aMuzzleLocation; }

	bool InitLaser(vec3 aLocation);

	void FireLaser(vec3 aLocation);

private:

};