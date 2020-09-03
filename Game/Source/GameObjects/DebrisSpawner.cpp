#include "GamePCH.h"

#include "DebrisSpawner.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/DebrisObject.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/AudioController.h"
#include "Helpers/TweenManager.h"

#include "Mesh/Material.h"
#include "Mesh/Mesh.h"

#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"

#include "Scenes/Scene.h"

DebrisSpawner::DebrisSpawner(Scene* pScene, Mesh* pMesh, Material* pMaterial, ResourceManager* pResources)
	:m_DebrisPool()
{
	m_pResources = pResources;
	m_pMyScene = pScene;

	//Create the debris objects for the pool
	for (int i = 0; i < MaxDebris; i++)
	{
		char tempstr[30]; //create a char string for the debris names
		sprintf_s(tempstr, 30, "Debris%d", i); //fill the char string

		//create the debris object
		GameObject* aDebris = new DebrisObject(m_pMyScene, tempstr, UID_ASTEROID, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1.0f, 1.0f, 1.0f), m_pResources->GetMesh("AsteroidMesh"), m_pResources->GetMaterial("AsteroidMaterial"), this);
		m_DebrisPool.AddNewObjectToPool(aDebris); //add the debris object to the pool
		aDebris->SetPool(&m_DebrisPool); //give it a reference to the pool
	}

}

DebrisSpawner::~DebrisSpawner()
{
	//delete any active debris objects
	for (auto pDebris : m_pActiveDebris)
	{
		delete pDebris;
	}
}

void DebrisSpawner::OnEvent(Event * pEvent)
{

}

void DebrisSpawner::Update(float deltaTime)
{
	//for every active debris object
	for (auto pDebris : m_pActiveDebris)
	{
		pDebris->Update(deltaTime); //update the debris object

		if (pDebris->GetLifeTime() <= 0.0f && DebrisIsActive(pDebris)) //if the debris objects life time is 0
		{
			pDebris->RemoveFromScene(); //remove it from the scene
		}
	}
}
void DebrisSpawner::Draw(GameCamera * aCamera)
{
	//draw each active debris object
	for (auto pDebris : m_pActiveDebris)
	{
		pDebris->Draw(aCamera);
	}
}

void DebrisSpawner::SpawnDebris(vec3 aPosition, float Radius)
{
	//each time debris is spawned it will spawn 40 pieces
	for (int i = 0; i < 40; i++)
	{
		GameObject* d = m_DebrisPool.GetObjectFromPool(); //get a debris object from the pool
		if (d)
		{
			float speed = 180.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (360.0f - 180.0f)); //get a random speed between 180.0f and 360.0f
			float scale = Radius / 10.0f; //the debris will be a tenth of the radius of the object being destroy

			//Get a Random X Y velocity within the bounds of the Space Bodies Radius
			float RadiusPercentage = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); //some random percentage between 0 and 1
			float AngleRads = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / ((2.0f * PI)))); //some random angle between 0 and 2 PI rads

			float SpawnOnRadius = Radius - sqrt((RadiusPercentage)* Radius); // Fix distribution and scale to [0, RealRadius]

			float X = cosf(AngleRads) * SpawnOnRadius;//find the x velocity
			float Y = sinf(AngleRads) * SpawnOnRadius;//find the y velocity

			//find a random z velocity between the destroyed object radius and negative radius
			float zSpeed = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (Radius * 2))) - Radius;

			//create the velocity vector with the x , y and z values
			vec3 velocity = vec3(X, Y, zSpeed);

			//set the position to the origin of the destroyed object
			d->SetPosition(aPosition);

			//normalize the velocity vector making it a direction vector
			velocity.Normalize();

			//apply it to the debris objects rotation/direction vector
			d->SetImpulseDirection(velocity * speed);

			//set the life time to the debris life time of 2
			d->SetLifeTime(2.0f);

			//set the debris objects scale to a tenth of the destroyed objects scale
			d->SetScale(vec3(scale, scale, scale));

			//add the debris object to the active debris vector
			m_pActiveDebris.push_back(d);
		}
	}

}

bool DebrisSpawner::RemoveDebris(GameObject* aDebris)
{
	auto iteratorForObject = std::find(m_pActiveDebris.begin(), m_pActiveDebris.end(), aDebris); //iterate through the active debris
	if (iteratorForObject != m_pActiveDebris.end()) //if the debris object is active
	{
		m_pActiveDebris.erase(iteratorForObject); //remove it from the active debris
		return true;
	}
	return false;
}

bool DebrisSpawner::DebrisIsActive(GameObject* aDebris)
{
	auto iteratorForObject = std::find(m_pActiveDebris.begin(), m_pActiveDebris.end(), aDebris); //iterate through the active debris
	if (iteratorForObject != m_pActiveDebris.end()) //if the debris object is active
	{
		return true;
	}
	return false;
}

void DebrisSpawner::Reset()
{
	//for each active debris remove it from the scene
	for (auto pDebris : m_pActiveDebris)
	{
		pDebris->RemoveFromScene();
	}
}

