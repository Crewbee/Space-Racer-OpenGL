#pragma once
#include "GameObjects/GameObject.h"

class Mesh;
class PlayerController;
class LaserRayObject;
enum PlayerState {
	PlayerStart = 0,
	PlayerRacing,
	PlayerWins,
	PlayerDead
};


class Player : public GameObject
{
protected:
    PlayerController* m_pPlayerController;
	LaserRayObject* m_pLasers;

    float m_Speed;
    float m_TurningSpeed;

	vec3 m_PlayerDirection;
	vec2 m_ControllerAxis;
	vec2 m_InBoundsAxis;

	bool m_UseLudicrousSpeed;
	bool m_pAccelarateStart;

	PlayerState m_pState;

public:
    Player(Scene* pScene, std::string name, CollisionID aUID, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~Player();

    virtual void Update(float deltaTime) override;

	virtual void Draw(GameCamera* aCamera);

    void SetPlayerController(PlayerController* controller) { m_pPlayerController = controller; }
    void SetSpeed(float speed) { m_Speed = speed; }
	void Accelarate(vec3 Direction, float deltaTime);
	void Deccelarate(vec3 Direction, float deltaTime);
	void PushRightLeft(float xAxis, float deltaTime);
	void PushUpDown(float yAxis, float deltaTime);
	bool InBounds(float aPlayerAxis, float aInBoundsAxis);

	PlayerState GetPlayerState() { return m_pState; }
	void SetPlayerState(PlayerState aState) { m_pState = aState; }

	virtual void SetControllerAxis(vec2 aControllerAxis);
	virtual void CheckForFireTarget();

	bool GetUseLudicrousSpeed() { return m_UseLudicrousSpeed;}
	void SetUseLudicrousSpeed(bool activateLudicrousSpeed) { m_UseLudicrousSpeed = activateLudicrousSpeed; }

	virtual void OnDeath() override;
	virtual void OnPlayerWin() override;

	virtual void Reset() override;

};
