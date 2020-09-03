#include "GamePCH.h"

#include "GameObject.h"

#include "GameObjects/GameCamera.h"

#include "Events/RemoveFromSceneEvent.h"
#include "Game/Game.h"

#include "Mesh/Material.h"
#include "Mesh/Mesh.h"

#include "Scenes/Scene.h"

#include "Physics3D/BulletManager.h"

GameObject::GameObject(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
	: m_Name(name)
	, m_pScene(pScene)
	, m_pMesh(pMesh)
	, m_pMaterial(pMaterial)
	, m_InitialPosition(pos)
	, m_InitialRotation(rot)
	, m_Position(pos)
	, m_Rotation(rot)
	, m_Scale(scale)
	, m_pRotatorAxis(vec3(0, 0, 0))
	, m_pRotatorSpeed(0.0f)
	, m_pBody(nullptr)
	, m_pCollisionID(aUID)
{
	m_UVScale.Set(1.0f, 1.0f);
	m_UVOffset.Set(0.0f, 0.0f);
	m_pSetImpluse = false;
    m_pTweenModifier = 1.0f;
	m_pIsActive = false;
	m_pIsTweening = false;
	m_pImpulseDirection.Set(0.0f, 0.0f, 0.0f);
	m_pLifeTime = 1.0f;

	m_WorldPosition.SetIdentity();
	m_WorldPosition.CreateSRT(m_Scale, vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z), m_Position);
}

GameObject::~GameObject()
{
}

void GameObject::Reset()
{
	m_UVScale.Set(1.0f, 1.0f);
	m_UVOffset.Set(0.0f, 0.0f);
	m_pSetImpluse = false;
	m_pTweenModifier = 1.0f;
	m_pIsActive = false;
	m_pIsTweening = false;
	m_pImpulseDirection.Set(0.0f, 0.0f, 0.0f);
	m_pLifeTime = 1.0f;
	m_pRotatorAxis.Set(0.0f, 0.0f, 0.0f);
	m_pRotatorSpeed = 0.0f;
	m_Position = m_InitialPosition;
	m_Rotation = m_InitialRotation;

	m_WorldPosition.SetIdentity();
	m_WorldPosition.CreateSRT(m_Scale, vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z), m_Position);
}

void GameObject::OnEvent(Event* pEvent)
{

}

void GameObject::Update(float deltaTime)
{
	if (m_pBody)
	{
		if (m_pBody->getActivationState() != 1)
		{
			m_pBody->setActivationState(1);
		}
		btTransform bodyLoc = m_pBody->getWorldTransform();

		btVector3 loc = bodyLoc.getOrigin();
		m_Position = vec3(loc.getX(), loc.getY(), loc.getZ());

		bodyLoc.setRotation(btQuaternion(-m_Rotation.y / 180.0f * PI, -m_Rotation.x / 180.0f * PI, -m_Rotation.z / 180.0f * PI));
		m_pBody->setWorldTransform(bodyLoc);
		m_pBody->setCenterOfMassTransform(bodyLoc);
	}

	vec3 newRotation = m_Rotation + (m_pRotatorAxis * m_pRotatorSpeed);

	m_Rotation = newRotation;

}

void GameObject::Draw(GameCamera* pCamera)
{
	if (m_pMesh != nullptr)
	{
		m_pMesh->SetupAttributes(m_pMaterial);

		m_WorldPosition.SetIdentity();
		m_WorldPosition.CreateSRT(m_Scale, vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z), m_Position);
		mat4 NormalMatrix;
		NormalMatrix.CreateRotation(vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));

		if (m_pMaterial->GetTexture() == nullptr)
		{
			m_pMesh->SetupDebugUniforms(m_pMaterial, m_WorldPosition, pCamera);
		}
		else if (GetName() == "UniverseSphere")
		{
			m_pMesh->SetupUniforms(m_pMaterial, m_WorldPosition, pCamera);
		}
		else if (m_pScene->GetHasLighting() == true)
		{
			m_pMesh->SetupUniformsWithLight(m_pMaterial, m_WorldPosition, NormalMatrix, pCamera, m_pScene->GetPointLights(), m_pScene->GetSpotLights());
		}
		else
		{
			m_pMesh->SetupUniforms(m_pMaterial, m_WorldPosition, pCamera);
		}
		m_pMesh->Draw(m_pMaterial);
	}
}

btQuaternion GameObject::CalculateQuaternionRotation(vec3 aRotation)
{
	// Abbreviations for the various angular functions
	float cy = cos(aRotation.x * 0.5);
	float sy = sin(aRotation.x * 0.5);
	float cp = cos(aRotation.y * 0.5);
	float sp = sin(aRotation.y * 0.5);
	float cr = cos(aRotation.z * 0.5);
	float sr = sin(aRotation.z * 0.5);

	float yaw = cy * cp * sr - sy * sp * cr;
	float pitch = sy * cp * sr + cy * sp * cr;
	float roll = sy * cp * cr - cy * sp * sr;
	btQuaternion q = btQuaternion(yaw, pitch, roll);
	return q;
}

vec3 GameObject::GetEulerRotation(vec3 aRotation)
{
	float RadX = aRotation.x * (PI / 180.0f);
	float RadY = aRotation.y * (PI / 180.0f);
	float RadZ = aRotation.z * (PI / 180.0f);

	return vec3(RadZ, RadY, RadX);
}
#undef new

void GameObject::CreateBody(btCollisionShape* aShape, btScalar Mass, bool isDynamic)
{
	/*collisionShapes.push_back(groundShape);*/
	btTransform bodyTransform;
	bodyTransform.setIdentity();
	bodyTransform.setOrigin(btVector3(m_Position.x, m_Position.y, m_Position.z));

	//rigid body is dynamic if and only if mass is non zero, otherwise static
	btScalar mass(Mass);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		aShape->calculateLocalInertia(mass, localInertia);
	}

	//using motion state is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, aShape, localInertia);
	m_pBody = new btRigidBody(rbInfo);
	float radius = GetMeshRadius();
	btBroadphaseProxy* collisionFilter = new btBroadphaseProxy(btVector3(-radius * 2, -radius * 2, -radius * 2), btVector3(radius * 2, radius * 2, radius * 2), m_pBody, 1, 1);
	collisionFilter->m_uniqueId = m_pCollisionID;
	m_pBody->setBroadphaseHandle(collisionFilter);

	m_pBody->setUserPointer(this);

	//add the body to the dynamics world
	m_pScene->GetBulletManager()->GetWorld()->addRigidBody(m_pBody);

}
void GameObject::CreateBoxBody(vec3 aSize, btScalar aMass, bool isDynamic)
{
	btCollisionShape* boxshape = new btBoxShape(btVector3(aSize.x, aSize.y, aSize.z));
	CreateBody(boxshape, aMass, isDynamic);
}
void GameObject::CreateHullBody(btScalar aMass, bool isDynamic)
{
	btConvexHullShape* hullshape = new btConvexHullShape();

	int numverts = m_pMesh->GetNumVerts();
	vec3* vertPos = m_pMesh->GetMeshPoints();

	for (int i = 0; i < numverts; i++)
	{
		{
			btVector3 newPoint = btVector3(vertPos[i].x * m_Scale.x, vertPos[i].y * m_Scale.y, vertPos[i].z * m_Scale.z);
			hullshape->addPoint(newPoint);
		}
	}

	btCollisionShape* ObjectHull = static_cast<btCollisionShape*>(hullshape);

	CreateBody(ObjectHull, aMass, isDynamic);

}

void GameObject::CreateSphereBody(btScalar aRadius, float aProportion, btScalar aMass, bool isDynamic)
{
	btCollisionShape* sphereBody = new btSphereShape(aRadius * aProportion);

	CreateBody(sphereBody, aMass, isDynamic);

}
#define new
void GameObject::SetPosition(vec3 pos)
{
	m_Position = pos;
}

void GameObject::SetRotation(vec3 rot)
{
	m_Rotation = rot;
}

void GameObject::SetScale(vec3 scale)
{
	m_Scale = scale;
}

void GameObject::SetUVScaleAndOffset(vec2 uvScale, vec2 uvOffset)
{
	m_UVScale = uvScale;
	m_UVOffset = uvOffset;
}

void GameObject::ReturnToPool()
{
}

bool GameObject::RemoveFromScene()
{
	return m_pScene->RemoveGameObject(this);
}

void GameObject::AddRotation(vec3 anAxis, float aSpeed)
{
	m_pRotatorAxis = anAxis;
	m_pRotatorSpeed = aSpeed;
}
float GameObject::GetMeshRadius()
{
	vec3 meshScaler = m_pMesh->GetMeshSize() * m_Scale;

	float diameter = meshScaler.x;

	if (meshScaler.y > diameter)
	{
		diameter = meshScaler.y;
	}
	if (meshScaler.z > diameter)
	{
		diameter = meshScaler.z;
	}

	return diameter / 2;
}
vec3 GameObject::GetMeshSize()
{
	return m_pMesh->GetMeshSize() * m_Scale;
}

vec3 GameObject::GetDirectionVector()
{
	return m_WorldPosition.GetAt();
}
