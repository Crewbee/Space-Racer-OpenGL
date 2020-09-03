#include "GamePCH.h"

#include "Events/GameEventTypes.h"
#include "Events/RemoveFromSceneEvent.h"

#include "Game/Game.h"
#include "Game/ImguiManager.h"

#include "GameObjects/GameObject.h"
#include "GameObjects/UIObject.h"
#include "GameObjects/SceneRenderer.h"
#include "GameObjects/GameCamera.h"
#include "GameObjects/OrthoCamera.h"

#include "Helpers/ResourceManager.h"
#include "Helpers/ResourceFactory.h"
#include "Helpers/AudioController.h"
#include "Helpers/SceneManager.h"
#include "Helpers/TweenManager.h"

#include "Scenes/Scene.h"

#include "Mesh/Material.h"
#include "Mesh/Texture.h"
#include "Mesh/FBO.h"

Game::Game(Framework* pFramework)
	: GameCore(pFramework, new EventManager())
{
	m_pImGuiManager = nullptr;

	m_pResources = nullptr;
	m_pResourceFactory = nullptr;
	m_pAudioController = nullptr;
	m_pSceneManager = nullptr;
	m_pTweenManager = nullptr;
	m_pFrameBufferObject = nullptr;

    m_pMyMode = JimmysJalopyMode;

}

Game::~Game()
{
	delete m_pSceneManager;
	delete m_pResourceFactory;
	delete m_pResources;
	delete m_pAudioController;
	delete m_pTweenManager;
	delete m_pImGuiManager;
	delete m_pFrameBufferObject;
}

void Game::OnSurfaceChanged(unsigned int width, unsigned int height)
{
	// Set OpenGL to draw to the entire window.
	if (height == 0)
	{
		height = 1;
	}
	float AspectRatio = (float)width / height;

	glViewport(0, 0, width, height);

	ResetFOV(45, vec2((float)width, (float)height), 50.0f, 50000.0f);
}

void Game::LoadContent()
{
#if WIN32
	// Turn on V-Sync.
	wglSwapInterval(1);
#endif

	// Turn on depth buffer testing.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Turn on alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable back-face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//Seed Random
	srand((int)GetSystemTimeSinceGameStart);

	// Create our resource manager.
	m_pResources = new ResourceManager();

	//Create our Audio Controller
	m_pAudioController = new AudioController(m_pResources);

	//Initialize AudioControllers sound Players for music and FX
	m_pAudioController->InitSoundPlayers();

	m_pAudioController->SetChannelVolume(0.5f, PlayerType::PT_MUSIC);
	m_pAudioController->SetChannelVolume(0.5f, PlayerType::PT_MISC);

	m_pFrameBufferObject = new FBO(1920, 1080, GL_LINEAR, GL_LINEAR, true);

	//Create our Resource Factory and add all Resources to the Manager
	m_pResourceFactory = new ResourceFactory(m_pResources, m_pAudioController, m_pFrameBufferObject);
	m_pResourceFactory->CreateResources();

	//Create our TweenManager
	m_pTweenManager = new TweenManager(this);

	// Create our scenes.
	m_pSceneManager = new SceneManager(this, m_pResources, m_pAudioController, m_pTweenManager, m_pFrameBufferObject);
	m_pSceneManager->LoadContent();

	// Push scenes.
	m_pSceneManager->PushScene(ST_GAMEPLAY);
	m_pSceneManager->PushScene(ST_HUD);
	m_pSceneManager->PushScene(ST_MENU);
	m_pSceneManager->PushScene(ST_SPLASH);

	// Create an ImGui object.
	//m_pImGuiManager = new ImGuiManager();
	//m_pImGuiManager->Init();

	CheckForGLErrors();
}

void Game::OnEvent(Event* pEvent)
{
	//m_pImGuiManager->OnEvent(pEvent);

	m_pSceneManager->OnEvent(pEvent);
	m_pAudioController->OnEvent(pEvent);

#if WIN32
	// Enable/Disable V-Sync with F1 and F2.
	if (pEvent->GetEventType() == EventType_Input)
	{
		InputEvent* pInput = static_cast<InputEvent*>(pEvent);

		// Enable V-Sync.
		if (pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == VK_F1)
			wglSwapInterval(1);

		// Disable V-Sync.
		if (pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == VK_F2)
			wglSwapInterval(0);
	}
#endif //WIN32
}

void Game::Update(float deltaTime)
{
	// Once our events are processed, tell ImGui we're starting a new frame.
	//m_pImGuiManager->StartFrame((float)m_pFramework->GetWindowWidth(), (float)m_pFramework->GetWindowHeight(), deltaTime);

	m_pSceneManager->Update(deltaTime);
}

void Game::Draw()
{
	// Setup the values we will clear to, then actually clear the color and depth buffers.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // (red, green, blue, alpha) - dark blue.
#if WIN32
	glClearDepth(1); // 1 is maximum depth.
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pSceneManager->Draw();

	// Tell imgui we're at the end of our frame, so it will draw all queued objects.
	//m_pImGuiManager->EndFrame();
	CheckForGLErrors();
}
void Game::ResetFOV(float angle, vec2 aspectRatio, float Znear, float Zfar)
{
	if (m_pSceneManager)
	{
		m_pSceneManager->ResetFOV(angle, aspectRatio, Znear, Zfar);
	}
}
