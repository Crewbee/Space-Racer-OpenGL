#include "GamePCH.h"

#include "Game/Game.h"
#include "Player.h"
#include "Mesh/Mesh.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/LaserRayObject.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/FollowCamera.h"
#include "GameObjects/DebrisSpawner.h"

#include "Scenes/Scene.h"

#include "Physics3D/BulletManager.h"
#include "Physics3D/BulletDebugDraw.h"

#include "Events/GameEventTypes.h"
#include "Events/AddScoreEvent.h"
#include "Events/SoundEvent.h"
#include "Events/RemoveFromSceneEvent.h"

Player::Player(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
	: GameObject(pScene, name, aUID, pos, rot, scale, pMesh, pMaterial)
	, m_pPlayerController(nullptr)
	, m_Speed(1500.0f)
{
	m_PlayerDirection.Set(0, 0, 1);
	m_ControllerAxis.Set(0, 0);
	m_InBoundsAxis.Set(125.0f, 125.0f);
	m_UseLudicrousSpeed = false;
	m_pState = PlayerStart;
	m_pLasers = new LaserRayObject(pScene, "Lasers", pos, rot, vec3(1.0f, 1.0f, 1.0f), nullptr, nullptr, this);
	m_pAccelarateStart = false;

}

Player::~Player()
{
	delete m_pLasers;
}

void Player::Update(float deltaTime)
{
	if (m_pState != PlayerDead)
	{
		GameObject::Update(deltaTime);
		switch (m_pState)
		{
		case PlayerStart:
			if (m_pBody)
			{
				m_pBody->getWorldTransform().setOrigin(btVector3(0.0f, 0.0f, -80.0f));

				Event* psound = new SoundEvent(Sounds::Idle, CanPlay);
				m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);

				if (m_pScene->GetSceneManager()->GetGameState() == Game_State::GS_RUNNING)
				{
					m_Rotation.Set(0.0f, 0.0f, 0.0f);
					if (m_pPlayerController->IsHeld_Up())
					{
						m_pState = PlayerRacing;
					}
				}
			}
			break;

		case PlayerRacing:
			if (m_UseLudicrousSpeed)
			{
				m_Speed = 2500.0f;
			}
			else
			{
				m_Speed = 1500.0f;
			}

			//ImGui::Text("Position_Player: %f %f %f", m_Position.x, m_Position.y, m_Position.z);


			if (m_pPlayerController)
			{
				if (m_pPlayerController->IsHeld_Up())
				{
					m_pBody->activate(true);
					if (m_pBody->getActivationState() != 1)
					{
						m_pBody->setActivationState(1);
					}
					Accelarate(m_PlayerDirection, deltaTime);
					if (m_pAccelarateStart == false)
					{
						Event* psound = new SoundEvent(Sounds::Accelarate, MustPlay);
						m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);
						m_pAccelarateStart = true;
					}
					else
					{
						Event* psound = new SoundEvent(Sounds::TopSpeed, CanPlay);
						m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);
					}
				}
				else
				{
					Deccelarate(m_PlayerDirection, deltaTime);
					if (m_pAccelarateStart == true)
					{
						Event* psound = new SoundEvent(Sounds::Deccelarate, MustPlay);
						m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);
						m_pAccelarateStart = false;
					}
				}
				if (m_pPlayerController->IsFiring() == 1)
				{
					CheckForFireTarget();
					Event* psound = new SoundEvent(Lasers_1, CanPlay);
					m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);
				}

				float distFromcenter = sqrt((m_Position.x * m_Position.x) + (m_Position.y * m_Position.y));

				if (distFromcenter < 250.0f)
				{
					PushRightLeft(m_ControllerAxis.x, deltaTime);
					PushUpDown(m_ControllerAxis.y, deltaTime);
				}
				else
				{
					float PushBackX = bool(m_Position.x < 0) * 2.0f - 1.0f;
					float PushBackY = bool(m_Position.y < 0) * 2.0f - 1.0f;

					PushRightLeft(PushBackX, deltaTime);
					PushUpDown(PushBackY, deltaTime);
				}

				m_pLasers->Update(deltaTime);
				break;
			}


		case PlayerWins:

			Deccelarate(m_PlayerDirection, deltaTime);
			break;
		}
	}
}

void Player::Draw(GameCamera * aCamera)
{
	if (m_pState != PlayerDead)
	{
		GameObject::Draw(aCamera);

		m_pLasers->Draw(aCamera);
	}
}

void Player::Accelarate(vec3 Direction, float deltaTime)
{
	btVector3 bodyDirection = btVector3(Direction.x, Direction.y, Direction.z);
	btScalar Mass = m_pBody->getInvMass();
	btVector3 bodyVelocity = m_pBody->getLinearVelocity();
	btVector3 m_DiffVelocity = (bodyDirection * m_Speed) - bodyVelocity;

	btVector3 Force = (Mass * m_DiffVelocity / deltaTime);

	m_pBody->applyImpulse(Force, m_pBody->getCenterOfMassPosition());
}

void Player::Deccelarate(vec3 Direction, float deltaTime)
{
	btVector3 bodyDirection = -btVector3(Direction.x, Direction.y, Direction.z);
	btScalar Mass = m_pBody->getInvMass();
	btVector3 bodyVelocity = m_pBody->getLinearVelocity();
	btVector3 m_DiffVelocity = (bodyDirection * bodyVelocity);

	btVector3 Force = (Mass * m_DiffVelocity / deltaTime);

	if (bodyVelocity.getZ() <= 0.0f)
	{
		Event* psound = new SoundEvent(Sounds::Idle, CanPlay);
		m_pScene->GetGame()->GetEventManager()->QueueEvent(psound);
	}
	else
	{
		m_pBody->applyImpulse(Force, m_pBody->getCenterOfMassPosition());
	}
}

void Player::PushRightLeft(float xAxis, float deltaTime)
{
	btVector3 bodyDirection = btVector3(xAxis, 0.0f, 0.0f);
	btScalar Mass = m_pBody->getInvMass();

	btVector3 Velocity = btVector3(m_pBody->getLinearVelocity().getX(), 0.0f, 0.0f);
	btVector3 m_DiffVelocity = (bodyDirection * (m_Speed * (4.0f * ((m_pBody->getLinearVelocity().getZ() / m_Speed)) + 1.0f))) - Velocity;
	btVector3 Force = (Mass * m_DiffVelocity / deltaTime);

	m_pBody->applyImpulse(Force, m_pBody->getCenterOfMassPosition());
}

void Player::PushUpDown(float yAxis, float deltaTime)
{
	btVector3 bodyDirection = btVector3(0.0f, yAxis, 0.0f);
	btScalar Mass = m_pBody->getInvMass();

	//ImGui::Text("yAxis %f", yAxis);

	btVector3 Velocity = btVector3(0.0f, m_pBody->getLinearVelocity().getY(), 0.0f);

	btVector3 m_DiffVelocity = (bodyDirection * (m_Speed * (4.0f * ((m_pBody->getLinearVelocity().getZ() / m_Speed)) + 1.0f))) - Velocity;

	btVector3 Force = (Mass * m_DiffVelocity / deltaTime);

	m_pBody->applyImpulse(Force, m_pBody->getCenterOfMassPosition());
}

bool Player::InBounds(float aPlayerAxis, float aInBoundsAxis)
{
	return (aPlayerAxis > -aInBoundsAxis && aPlayerAxis < aInBoundsAxis);
}

void Player::SetControllerAxis(vec2 aControllerAxis)
{
	m_ControllerAxis = aControllerAxis;
}

void Player::CheckForFireTarget()
{
	vec3 ShipDirection = GetDirectionVector();
	ShipDirection.Normalize();

	btVector3 btFrom = btVector3(ShipDirection.x, ShipDirection.y, ShipDirection.z) * GetMeshSize().z + btVector3(m_Position.x, m_Position.y, m_Position.z);
	btVector3 btTo = btVector3(ShipDirection.x, ShipDirection.y, ShipDirection.z) * 4000.0f + btVector3(m_Position.x, m_Position.y, m_Position.z);

	btVector3 points[2] = { btFrom, btTo };
	m_pScene->GetBulletManager()->GetDebugDrawer()->AddRayPoints(&points[0], 2);

	m_pLasers->SetMuzzleLocation(vec3(btFrom.getX(), btFrom.getY(), btFrom.getZ()));
	m_pLasers->FireLaser(vec3(btTo.getX(), btTo.getY(), btTo.getZ()));

	btCollisionWorld::ClosestRayResultCallback rayCallback(btFrom, btTo);

	m_pScene->GetBulletManager()->GetWorld()->rayTest(btFrom, btTo, rayCallback); // m_btWorld is btDiscreteDynamicsWorld

	if (rayCallback.hasHit())
	{
		GameObject* p = reinterpret_cast<GameObject*>(btRigidBody::upcast(rayCallback.m_collisionObject)->getUserPointer());

		if (p)
		{
			if (p->GetCollisionID() == UID_ASTEROID)
			{
				p->ApplyDamage(1);
			}
		}
	}
}

void Player::OnDeath()
{
	m_pScene->GetCamera()->DetachCamera();
	m_pScene->GetDebrisSpawner()->SpawnDebris(m_Position, GetMeshRadius() * 50.0f);

	Event* remove = new RemoveFromSceneEvent(m_pScene, this);
	m_pScene->GetGame()->GetEventManager()->QueueEvent(remove);

	m_pState = PlayerDead;

	m_pScene->GetSceneManager()->SetGameState(Game_State::GS_GAMEOVER);
}

void Player::OnPlayerWin()
{
	m_pScene->GetCamera()->DetachCamera();

	m_pScene->GetSceneManager()->SetGameState(Game_State::GS_WINGAME);

	m_pState = PlayerWins;

}

void Player::Reset()
{
	SetIsActive(true);
	if (m_pState != PlayerWins)
	{
		if (m_pBody)
		{
			delete m_pBody;
		}

		CreateHullBody(100.0f, true);
	}
	GetBody()->getWorldTransform().setOrigin(btVector3(0.0f, 0.0f, -80.0f));
	GetBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	GetBody()->getWorldTransform().setRotation(btQuaternion(0.0f, 0.0f, 0.0f));
	SetRotation(vec3(0.0f, 0.0f, 0.0f));
	m_pState = PlayerStart;
    m_UseLudicrousSpeed = false;
	m_pScene->GetCamera()->SetObjectToFollow(this, vec3(0, 1, -8));
	m_pScene->GetGameObjectByName("UniverseSphere")->SetPosition(m_Position);

}

