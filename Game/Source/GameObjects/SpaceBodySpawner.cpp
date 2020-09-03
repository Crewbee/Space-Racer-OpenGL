#include "GamePCH.h"

#include "GameObjects/GameObject.h"
#include "SpaceBodySpawner.h"
#include "GameObjects/Player.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Planetoid.h"

#include "Helpers/ResourceManager.h"
#include "Physics3D/BulletManager.h"
#include "Scenes/Scene.h"

SpaceBodySpawner::SpaceBodySpawner(Scene * pScene, ResourceManager * pResources, vec3 pos)
	: GameObject(pScene, "Spawner", UID_NULL, pos, vec3(0, 0, 0), vec3(1, 1, 1), nullptr, nullptr)
	, m_Asteroids()
	, m_Planetoids()
{
	m_AsteroidCount = 500;
	m_PlanetoidCount = 50;

	m_DistAsteroidSpawn = 120;

	m_DistPlanetoidSpawn = 500;
	m_pResources = pResources;

	m_Offset = 8000.0f;

	m_CheckZChanged = (int)m_Position.z;

	PlanetoidSpawnTimer = 2800.0f;
	AsteroidSpawnTimer = 1200.0f;

	//the asteroids first spawn point is 1200 units in front of the player
	m_pNextAsteroidSpawnPos = m_pScene->GetPlayer()->GetPosition().z + m_Offset + AsteroidSpawnTimer;

	//the planetoids first spawn point is 2800 units in front of the player
	m_pNextPlanetoidSpawnPos = m_pScene->GetPlayer()->GetPosition().z + m_Offset + PlanetoidSpawnTimer;

}

SpaceBodySpawner::~SpaceBodySpawner()
{
	m_Asteroids.~ObjectPool();
	m_Planetoids.~ObjectPool();
}

void SpaceBodySpawner::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	vec3 UpdatedPos = m_pScene->GetPlayer()->GetPosition(); //get the current player position

	SetPosition(vec3(0.0f, 0.0f, UpdatedPos.z + m_Offset)); //add the spawners offset to it

	if (m_Position.z > m_pNextPlanetoidSpawnPos)
	{
		SpawnPlanetoids();
		m_pNextPlanetoidSpawnPos = m_Position.z + PlanetoidSpawnTimer;
	}
	if (m_Position.z > m_pNextAsteroidSpawnPos)
	{
		SpawnAsteroids();
		m_pNextAsteroidSpawnPos = m_Position.z + AsteroidSpawnTimer;
	}
}

void SpaceBodySpawner::SpawnAsteroids()
{
	GameObject* asteroid = m_Asteroids.GetObjectFromPool();
	if (asteroid)
	{
		//Get a Random Position within the in bounds of the player at the spawners Z position
		float RadiusPercentage = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);; //some random percentage between 0 and 1
		float AngleRads = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / ((2.0f * PI)))); //some random angle between 0 and 2 PI rads

		float SpawnOnRadius = 120.0f - sqrt((RadiusPercentage) * 120.0f); // Fix distribution and scale to [0, RealRadius]

		float Degrees = AngleRads * 180 / PI;//Calculate degrees

		float x = cosf(AngleRads) * SpawnOnRadius;//find the x position in world space
		float y = sinf(AngleRads) * SpawnOnRadius;//find the y position in world space

		vec3 AsertoidPos = vec3(x, y, m_Position.z);//add the z position of the spawner

		if (asteroid->GetBody() == nullptr)
		{
			asteroid->CreateHullBody(asteroid->GetMeshRadius() * 2, true);
		}
		else
		{
			m_pScene->GetBulletManager()->GetWorld()->addCollisionObject(asteroid->GetBody());
		}
		asteroid->GetBody()->setActivationState(1);
		asteroid->GetBody()->activate();//activate Body
		asteroid->GetBody()->getWorldTransform().setOrigin(btVector3(AsertoidPos.x, AsertoidPos.y, AsertoidPos.z));//SetPosition

		asteroid->SetImpulseDirection(vec3(0, 0, 0));

		m_pScene->AddGameObject(asteroid);//add to scene
	}
}

void SpaceBodySpawner::SpawnPlanetoids()
{
	GameObject* planetoid = m_Planetoids.GetObjectFromPool();
	if (planetoid)
	{
		//Get a Random Position on the in bounds of the player at the spawner Z position
		float AngleRads = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / ((2.0f * PI))));//random angle between 0 and 2 PI rads
		float SpawnRadius = 120.0f + (planetoid->GetMeshRadius());

		float Degrees = AngleRads * 180 / PI;//Calculate degrees

		float x = SpawnRadius * cosf(AngleRads);//find the x position in world space from origin 0
		float y = SpawnRadius * sinf(AngleRads);//find the y position in world space from origin 0

		vec3 PlanetoidPos = vec3(x, y, m_Position.z);//add the z position of the spawner
		

		if (planetoid->GetBody() == nullptr)
		{
			planetoid->CreateSphereBody(planetoid->GetMeshRadius(), 1.0f, planetoid->GetMeshRadius() * 2, true); //create a sphere body
		}
		else
		{
			m_pScene->GetBulletManager()->GetWorld()->addCollisionObject(planetoid->GetBody()); //add it's body back to the collision world
		}
		planetoid->GetBody()->getWorldTransform().setOrigin(btVector3(m_Position.x, m_Position.y, m_Position.z));
		planetoid->GetBody()->setActivationState(1);
		planetoid->GetBody()->getWorldTransform().setOrigin(btVector3(PlanetoidPos.x, PlanetoidPos.y, PlanetoidPos.z));//SetPosition

		m_pScene->AddGameObject(planetoid);//add to scene
	}
}

void SpaceBodySpawner::LoadContent()
{
	for (int i = 0; i < m_AsteroidCount; i++)
	{
		float AsteroidScale = 40.0f + (rand() % 120); //random scale modifier
		char tempstr[30];//create a char array for the asteroids specific name
		sprintf_s(tempstr, 30, "Asteroid%d", i); //load the array

		Asteroid* aAsteroid = new Asteroid(m_pScene, tempstr, UID_ASTEROID, m_Position, vec3(0, 0, 0), vec3(AsteroidScale, AsteroidScale, AsteroidScale),
			m_pResources->GetMesh("AsteroidMesh"), m_pResources->GetMaterial("AsteroidMaterial")); //create the asteroid object
		m_Asteroids.AddNewObjectToPool(aAsteroid); //add it to the pool
		aAsteroid->SetPool(&m_Asteroids); //set the asteroids pool reference to the asteroid pool
	}
	for (int i = 0; i < m_PlanetoidCount; i++)
	{
		float Planetoidscale = 5.0f + (rand() % 15); //random scale modifier
		char tempstr[30]; //create a char array for the planetoids specific name
		sprintf_s(tempstr, 30, "Planetoid%d", i);

		Planetoid* aPlanetoid = new Planetoid(m_pScene, tempstr, UID_ASTEROID, m_Position, vec3(0, 0, 0), vec3(Planetoidscale, Planetoidscale, Planetoidscale),
			m_pResources->GetMesh("MoonMesh"), m_pResources); //create the planetoid object
		m_Planetoids.AddNewObjectToPool(aPlanetoid); //add it to the pool
		aPlanetoid->SetPool(&m_Planetoids); //set the asteroids pool reference to the asteroid pool
	}

	m_Position.z += m_Offset; //push the spawner out in the z direction by it's offset
}

void SpaceBodySpawner::Reset()
{

}
