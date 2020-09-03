#include "GamePCH.h"

#include "BulletManager.h"
#include "Physics3D/BulletDebugDraw.h"
#include "GameObjects/GameObject.h"

//Undefine our new operator in order to allow Bullet new to work,will no show mem leaks
#undef new;

BulletManager::BulletManager()
{
    //Create Bullet3D Physics world

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.

    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();



    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)

    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);



    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.

    m_pOverlappingPairCache = new btDbvtBroadphase();



    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)

    m_pSolver = new btSequentialImpulseConstraintSolver;



    m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pOverlappingPairCache, m_pSolver, m_pCollisionConfiguration);
    m_pDynamicsWorld->setGravity(btVector3(0, 0, 0));

    gContactStartedCallback = ContactStartedCallback;
    gContactEndedCallback = ContactEndedCallback;

}

BulletManager::~BulletManager()
{
    delete m_pDynamicsWorld;
    delete m_pSolver;
    delete m_pOverlappingPairCache;
    delete m_pDispatcher;
    delete m_pCollisionConfiguration;
}

void BulletManager::SetupDebugDraw(BulletDebugDraw* aDebugDrawer)
{
    m_pDynamicsWorld->setDebugDrawer(aDebugDrawer);
    m_pDebugDrawer = aDebugDrawer;
}

void BulletManager::Update(float deltatime)
{
    m_pDynamicsWorld->stepSimulation(deltatime, 10);
    m_pDynamicsWorld->performDiscreteCollisionDetection();

    int numManifolds = m_pDynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold = m_pDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);

            const btVector3& ptA = pt.getPositionWorldOnA();
            const btVector3& ptB = pt.getPositionWorldOnB();

            GameObject* collisionA = reinterpret_cast<GameObject*>(contactManifold->getBody0()->getUserPointer());
            GameObject* collisionB = reinterpret_cast<GameObject*>(contactManifold->getBody1()->getUserPointer());

			if (collisionA->GetName() == "SpaceShip")
			{
				collisionA->OnDeath();
			}
			else if (collisionB->GetName() == "SpaceShip")
			{
				collisionB->OnDeath();
			}
        }
    }
}

btDiscreteDynamicsWorld * BulletManager::GetWorld()
{
    return m_pDynamicsWorld;
}

void BulletManager::ContactStartedCallback(btPersistentManifold * const & manifold)
{

}

void BulletManager::ContactEndedCallback(btPersistentManifold * const & manifold)
{

}
