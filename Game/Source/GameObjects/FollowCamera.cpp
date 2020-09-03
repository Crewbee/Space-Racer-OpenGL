#include "GamePCH.h"

#include "GameObjects/GameCamera.h"
#include "FollowCamera.h"
#include "GameObjects/GameObject.h"

#include "Game/Game.h"

#include "Scenes/Scene.h"

#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

FollowCamera::FollowCamera(Scene* aScene, std::string aName, vec3 pos, vec3 rot, vec3 scale, vec3 aTarget)
	: GameCamera(aScene, aName, pos, rot, scale, aTarget)
{
	// the offset of the camera relative to its follow object
	m_pCameraOffset = vec3(0, 0, 0);

	//sets whether the mouse position will affect the target object
	m_MouseLocked = true;

	//vector that holds the difference in the mouse position each frame
	m_MouseChangeSinceLastFrame.Set(0, 0);

	//vector holding the last position of the mouse
	m_LastMousePosition.Set(0, 0);

	//a vector to hold a lagged mouse position which is used to control the target
	m_MouseOffset.Set(0, 0); 

	//initialize a rotation vector for the target object
	m_TargetRotation = vec3(0, 0, 0);

	//initialize the reference to the target object
	m_LookAtTarget = aTarget;

	//initialize the vector that will offset the position the camera will look at
	m_LookAtOffset = vec3(0, 0, 0);

	//initialize the cameras view matrix
	m_pViewMatrix.SetIdentity();

	//create a view, look at matrix 
	m_pViewMatrix.CreateLookAtView(pos, vec3(0, 1, 0), aTarget);
}

FollowCamera::~FollowCamera()
{
	if (m_pTargetObject != nullptr)
	{
		m_pTargetObject = nullptr;
	}
}

void FollowCamera::OnEvent(Event * pEvent)
{
	//if the game is beginning or running
	if (m_pScene->GetSceneManager()->GetGameState() == Game_State::GS_RUNNING || m_pScene->GetSceneManager()->GetGameState() == Game_State::GS_BEGINPLAY)
	{
		//cast the event to an input event
		InputEvent* pInput = static_cast<InputEvent*>(pEvent);

		//if it is a keyboard (at the moment there is no support for input devices other than mouse/keyboard)
		if (pInput->GetInputDeviceType() == InputDeviceType_Keyboard)
		{
			//get the key ID
			int keyID = pInput->GetID();

			//if the key is being pressed
			if (pInput->GetInputState() == InputState_Pressed)
			{
				//if the key is the 'C' key
				if (keyID == 'C')
				{
					//if the mouse is controlling the target object
					if (m_MouseLocked)
					{
						m_MouseLocked = false; //set it to not control the target object
						m_MouseChangeSinceLastFrame.Set(0, 0); //zero the mouse position change vector
						m_MouseOffset.Set(m_WindowSize.x / 2, m_WindowSize.y / 2); //set the mouse offset vector to the middle of the window
						m_LastMousePosition.Set(m_WindowSize.x / 2, m_WindowSize.y / 2); //set the mouse position vector to the middle of the window
						SetCursorPos((int)m_LastMousePosition.x, (int)m_LastMousePosition.y); //set the cursor position to the middle of the window

					}
					//if the mouse is not controlling the target object
					else
					{
						m_MouseLocked = true; //set it to control the target object
						m_MouseChangeSinceLastFrame.Set(0, 0); //zero the mouse position change vector
						m_MouseOffset.Set(m_WindowSize.x / 2, m_WindowSize.y / 2); //set the mouse offset vector to the middle of the window
						m_LastMousePosition.Set(m_WindowSize.x / 2, m_WindowSize.y / 2); //set the mouse position vector to the middle of the window
						SetCursorPos((int)m_LastMousePosition.x, (int)m_LastMousePosition.y); //set the cursor position to the middle of the window
					}
				}
			}
		}
		//if the input device is mouse (at the moment there is no support for input devices other than mouse/keyboard)
		if (pInput->GetInputDeviceType() == InputDeviceType_Mouse)
		{
			//if the mouse is controlling the target object
			if (m_MouseLocked)
			{
				m_MouseChangeSinceLastFrame = m_LastMousePosition - pInput->GetPosition(); //update the mouse position change since last frame vector

				if (m_MouseChangeSinceLastFrame != vec2(0, 0)) //if the mouse has moved
				{
					//set the mouse position vector to the cursor position in negative to position space
					m_LastMousePosition = vec2(pInput->GetPosition().x - m_WindowSize.x / 2, pInput->GetPosition().y - m_WindowSize.y / 2);
				}
			}
		}
	}
}

void FollowCamera::Update(float deltatime)
{
	GameCamera::Update(deltatime);

	//if the target is not null
	if (m_pTargetObject != nullptr)
	{
		//initialize a target direction vector
		vec3 TargetDir = vec3(0, 0, 0);

		//if the mouse is controlling the target
		if (m_MouseLocked == true)
		{
			//create a local mouse change vector to increment the mouse offset to half of the mouse actual location in order to smooth the response of the target
			vec2 MouseChange = vec2((m_LastMousePosition.x - m_MouseOffset.x) / 2, (m_LastMousePosition.y - m_MouseOffset.y)) / 2;

			//apply the change amount to the offset vector
			m_MouseOffset += MouseChange;

			//if the game state is running
			if (m_pScene->GetSceneManager()->GetGameState() == Game_State::GS_RUNNING)
			{
				TargetDir = SetTargetRotation(); //set the rotation of the target object using the screen space position of the mouse
			}
		}
		//if the mouse is not controlling the target
		else
		{
			m_pTargetObject->SetRotation(vec3(0, 0, 0)); //set the targets rotation to 0
		}

		//add a magnitude of 4 to the direction vector of the target
		m_LookAtTarget = m_pTargetObject->GetPosition() + (m_pTargetObject->GetDirectionVector() * 4.0f);

		//update the position of the follow camera
		m_Position = m_pTargetObject->GetPosition() + vec3(m_pCameraOffset.x, m_pCameraOffset.y, m_pCameraOffset.z);

		//update the view matrix with the camera's new position and the new look at point
		m_pViewMatrix.CreateLookAtView(m_Position, m_pUpVector, m_LookAtTarget);



	}
	//if the target is null
	else
	{
		//if the player has lost
		if (m_pScene->GetSceneManager()->GetGameState() == Game_State::GS_GAMEOVER)
		{
			//increment the camera position by the negative of its look at vector at a magnitude of 2
			m_Position -= m_LookAtTarget.GetNormalized() * 2.0f;
		}
		//if the player has won
		else if (m_pScene->GetSceneManager()->GetGameState() == Game_State::GS_WINGAME)
		{
			//increment the camera position by the negative of its look at vector at a magnitude of 3.4
			m_Position -= m_LookAtTarget.GetNormalized() * 3.4f;
		}
		//update the view matrix with the new camera position
		m_pViewMatrix.CreateLookAtView(m_Position, m_pUpVector, m_LookAtTarget);
	}
}

void FollowCamera::SetObjectToFollow(GameObject* aTarget, vec3 anOffset)
{
	m_pTargetObject = aTarget; //set the reference pointer of the target object
	m_pViewMatrix.CreateLookAtView((aTarget->GetPosition() + anOffset), m_pUpVector, aTarget->GetDirectionVector() * 4.0f); //reset the view matrix
	m_pCameraOffset = anOffset; //reset the cameras position offset
}

vec3 FollowCamera::SetTargetRotation()
{
	m_TargetRotation = vec3(0, 0, 0); //zero the target rotation vector
	float mouseX = m_MouseOffset.x; //create a local float for the mouse x position
	float mouseY = -m_MouseOffset.y; //create a local float for the mouse y position and flip it

	float Xpercentage = mouseX / (m_WindowSize.y / 2);
	float Ypercentage = mouseY / (m_WindowSize.y / 2);

	MyClamp(Xpercentage, -1.0f, 1.0f);
	MyClamp(Ypercentage, -1.0f, 1.0f);

	m_TargetRotation.x = sinf((60 * Ypercentage) * (PI / 180.0f)) * (180.0f / PI);
	m_TargetRotation.y = -(sinf((20 * Xpercentage) * (PI / 180.0f)) * (180.0f / PI));
	m_TargetRotation.z = sinf((80 * Xpercentage) * (PI / 180.0f)) * (180 / PI);


	vec3 currentRotation = m_pTargetObject->GetRotation();

	float diff = sqrt((m_TargetRotation.x - currentRotation.x) * (m_TargetRotation.x - currentRotation.x) + (m_TargetRotation.y - currentRotation.y) * (m_TargetRotation.y - currentRotation.y) + (m_TargetRotation.z - currentRotation.z) * (m_TargetRotation.z - currentRotation.z));

	if (diff > 20.0f)
	{
		m_pTargetObject->SetRotation(currentRotation);
		return currentRotation;
	}
	m_pTargetObject->SetControllerAxis(vec2(Xpercentage, Ypercentage));
	m_pTargetObject->SetRotation(m_TargetRotation);

	return m_TargetRotation;
}

void FollowCamera::DetachCamera()
{
	m_pTargetObject = nullptr;
}

void FollowCamera::Reset()
{
	m_TargetRotation = vec3(0.0f, 0.0f, 0.0f);
	m_LastMousePosition.Set(m_WindowSize.x / 2, m_WindowSize.y / 2);
	m_MouseChangeSinceLastFrame.Set(0.0f, 0.0f);
	SetCursorPos((int)m_WindowSize.x / 2, (int)m_WindowSize.y / 2);
}
