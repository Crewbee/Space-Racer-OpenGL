#pragma once
#include "../Framework/Libraries/Bullet3/src/LinearMath/btIDebugDraw.h"

class GameCamera;
class ResourceManager;
class Material;

using namespace std;
class BulletDebugDraw : public btIDebugDraw
{
protected:

	int m_debugMode;
	ResourceManager* m_Resources;
	GameCamera* m_Camera;
	Material* m_DebugMaterial;
	mat4 m_DebugWorlMatrix;

	vector<btVector3> m_Raypoints;


public:
	BulletDebugDraw(ResourceManager* aResourceManager, GameCamera* aCamera);
	virtual ~BulletDebugDraw();

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void reportErrorWarning(const char* warningString);

	virtual void draw3dText(const btVector3& location, const char* textString);

	virtual void setDebugMode(int debugMode);

	virtual int  getDebugMode() const { return m_debugMode; }

	virtual void AddRayPoints(btVector3* aRayPoints, int numPoints);

	bool IsRaypoints() { return (!m_Raypoints.empty()); }

	virtual void DrawRays();

private:

};