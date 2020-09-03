#pragma once
class BulletDebugDraw;

class BulletManager
{
protected:
	btDefaultCollisionConfiguration* m_pCollisionConfiguration;
	btCollisionDispatcher* m_pDispatcher;
	btBroadphaseInterface* m_pOverlappingPairCache;
	btSequentialImpulseConstraintSolver* m_pSolver;
	btDiscreteDynamicsWorld* m_pDynamicsWorld;
	BulletDebugDraw* m_pDebugDrawer;
public:
	BulletManager();
	~BulletManager();

	void SetupDebugDraw(BulletDebugDraw* aDebugDrawer);

	BulletDebugDraw* GetDebugDrawer() { return m_pDebugDrawer; }

	void Update(float deltatime);

	btDiscreteDynamicsWorld* GetWorld();

	static void ContactStartedCallback(btPersistentManifold* const& manifold);
	static void ContactEndedCallback(btPersistentManifold* const& manifold);


private:

};
