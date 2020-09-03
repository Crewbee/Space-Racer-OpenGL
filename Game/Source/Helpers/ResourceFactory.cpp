#include "GamePCH.h"

#include "ResourceFactory.h"
#include "Helpers/ResourceManager.h"
#include "Mesh/Material.h"
#include "Mesh/Mesh.h"
#include "Mesh/Texture.h"
#include "Mesh/FBO.h"
#include "GameObjects/PlayerController.h"
#include "XAudio2/SoundPlayer.h"
#include "XAudio2/SoundObject.h"
#include "Helpers/AudioController.h"
#include "Events/SoundEvent.h"

ResourceFactory::ResourceFactory(ResourceManager * aResourceManager, AudioController* aAudioController, FBO* aFBO)
{
	m_pResources = aResourceManager;
	m_pAudio = aAudioController;
    m_FBO = aFBO;
}

ResourceFactory::~ResourceFactory()
{

}

void ResourceFactory::CreateResources()
{
	// Create our Resources.
	
	//Load the Shaders
	ShaderProgram* TextureShader				= new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag");
	ShaderProgram* MainShader					= new ShaderProgram("Data/Shaders/Main.vert", "Data/Shaders/Main.frag");
	ShaderProgram* DebugDrawShader				= new ShaderProgram("Data/Shaders/DebugDraw.vert", "Data/Shaders/DebugDraw.frag");
	ShaderProgram* LampShader					= new ShaderProgram("Data/Shaders/LampShader.vert", "Data/Shaders/LampShader.frag");
	ShaderProgram* PostRenderShader				= new ShaderProgram("Data/Shaders/PostRender.vert", "Data/Shaders/PostRender.frag");
	ShaderProgram* CubeMapShader				= new ShaderProgram("Data/Shaders/CubeMap.vert", "Data/Shaders/CubeMap.frag");

	// Load the textures.
	const char* cubeMapFileNames[6] = {
	"Data/Textures/negx.png",
	"Data/Textures/negy.png",
	"Data/Textures/negz.png",
	"Data/Textures/posx.png",
	"Data/Textures/posy.png",
	"Data/Textures/posz.png"
	};
	//CubeMap Textures
	Texture* cubemap							= new Texture(cubeMapFileNames, "Data/Textures/2k_ceres.png");

	//Object Textures
	Texture* MoonTexture						= new Texture("Data/Textures/2k_moon.png");
	Texture* SpaceShipTexture					= new Texture("Data/Textures/ShipTexture.png");
	Texture* AsteriodTexture					= new Texture("Data/Textures/Asteroid.png");
	Texture* UniverseTexture					= new Texture("Data/Textures/4k_stars.png");
	Texture* VenusTexture						= new Texture("Data/Textures/2k_venus_surface.png");
	Texture* CeresTexture						= new Texture("Data/Textures/2k_ceres.png");
	Texture* ErisTexture						= new Texture("Data/Textures/2k_eris.png");
	Texture* HaumealTexture						= new Texture("Data/Textures/2k_haumeal.png");
	Texture* MakemakeTexture					= new Texture("Data/Textures/2k_makemake.png");
	Texture* MarsTexture						= new Texture("Data/Textures/2k_mars.png");
	Texture* NeptuneTexture						= new Texture("Data/Textures/2k_neptune.png");
	Texture* SunTexture							= new Texture("Data/Textures/2k_sun.png");
	Texture* RingTexture						= new Texture("Data/Textures/Ring_Texture.png");
	Texture* FinishLineTexture					= new Texture("Data/Textures/FinishLineTexture.png");

	//Number textures
	Texture* Zero								= new Texture("Data/Textures/N0.png");
	Texture* One								= new Texture("Data/Textures/N1.png");
	Texture* Two								= new Texture("Data/Textures/N2.png");
	Texture* Three								= new Texture("Data/Textures/N3.png");
	Texture* Four								= new Texture("Data/Textures/N4.png");
	Texture* Five								= new Texture("Data/Textures/N5.png");
	Texture* Six								= new Texture("Data/Textures/N6.png");
	Texture* Seven								= new Texture("Data/Textures/N7.png");
	Texture* Eight								= new Texture("Data/Textures/N8.png");
	Texture* Nine								= new Texture("Data/Textures/N9.png");

	//UI Object Textures
	Texture* ExitButtonTexture					= new Texture("Data/Textures/ExitButtonTexture.png");
	Texture* SelectExitTexture					= new Texture("Data/Textures/SelectExitTexture.png");
	Texture* OptionsButtonTexture				= new Texture("Data/Textures/OptionsButtonTexture.png");
	Texture* SelectOptionsTexture				= new Texture("Data/Textures/SelectOptionsTexture.png");
	Texture* PlayButtonTexture					= new Texture("Data/Textures/PlayerButtonTexture.png");
	Texture* SelectPlayTexture					= new Texture("Data/Textures/SelectPlayTexture.png");
	Texture* PressEnterTexture					= new Texture("Data/Textures/PressEnterTexture.png");
	Texture* NoMusicTexture						= new Texture("Data/Textures/NoMusicTexture.png");
	Texture* SelectNoMusicTexture				= new Texture("Data/Textures/SelectNoMusic.png");
	Texture* LudicrousSpeedTexture				= new Texture("Data/Textures/LudicrousSpeedTexture.png");
	Texture* SelectLudicrousSpeedTexture		= new Texture("Data/Textures/SelectLudicrousSpeed.png");
	Texture* MainMenuTexture					= new Texture("Data/Textures/MainMenuTexture.png");
	Texture* SelectMainMenuTexture				= new Texture("Data/Textures/SelectMainMenu.png");
	Texture* AudioButtonTexture					= new Texture("Data/Textures/AudioButton.png");
	Texture* SelectAudioTexture					= new Texture("Data/Textures/SelectAudio.png");
	Texture* AudioTitleTexture					= new Texture("Data/Textures/AudioTitle.png");
	Texture* AudioMusicSliderTexture			= new Texture("Data/Textures/MusicSliderTexture.png");
	Texture* AudioFXSliderTexture				= new Texture("Data/Textures/FXSliderTexture.png");
	Texture* SliderNubTexture					= new Texture("Data/Textures/SliderNubTexture.png");
	Texture* GameOverTitleTexture				= new Texture("Data/Textures/GameOverTitle.png");
	Texture* ContinueButtonTexture				= new Texture("Data/Textures/ContinueButton.png");
	Texture* SelectContinueTexture				= new Texture("Data/Textures/SelectContinue.png");
	Texture* PauseTitleTexture					= new Texture("Data/Textures/PauseTitle.png");
	Texture* ScoreTitleTexture					= new Texture("Data/Textures/ScoreTitle.png");
	Texture* GoTexture							= new Texture("Data/Textures/Go.png");
	Texture* WinTexture							= new Texture("Data/Textures/WinTitle.png");
	Texture* UITexture							= new Texture("Data/Textures/UITexture.png");

	//Render to Texture
	Texture* RenderTexture						= new Texture(m_FBO->GetColorTextureHandle());

	//Create the Materials

	//CubeMap Material
	Material* CubeMapMaterial					= new Material(CubeMapShader, cubemap);

	//Game Object Materials
	Material* CamMat							= new Material(TextureShader, nullptr);
	Material* WhiteLampMaterial					= new Material(LampShader, MyColor(255, 255, 255, 255));
	Material* RedLampMaterial					= new Material(LampShader, MyColor(255, 100, 100, 255));
	Material* GreenLampMaterial					= new Material(LampShader, MyColor(100, 255, 100, 255));
	Material* BlueLampMaterial					= new Material(LampShader, MyColor(100, 100, 255, 255));
	Material* MoonMaterial						= new Material(MainShader, MoonTexture);
	Material* ShipMaterial						= new Material(MainShader, SpaceShipTexture);
	Material* AsteroidMaterial					= new Material(MainShader, AsteriodTexture);
	Material* VenusMaterial						= new Material(MainShader, VenusTexture);
	Material* CeresMaterial						= new Material(MainShader, CeresTexture);
	Material* ErisMaterial						= new Material(MainShader, ErisTexture);
	Material* HaumealMaterial					= new Material(MainShader, HaumealTexture);
	Material* MakemakeMaterial					= new Material(MainShader, MakemakeTexture);
	Material* MarsMaterial						= new Material(MainShader, MarsTexture);
	Material* NeptuneMaterial					= new Material(MainShader, NeptuneTexture);
	Material* SunMaterial						= new Material(MainShader, SunTexture);
	Material* RingMaterial						= new Material(MainShader, RingTexture);
	Material* FinishLineMaterial				= new Material(MainShader, FinishLineTexture);
	Material* UniverseMaterial					= new Material(TextureShader, UniverseTexture);

	//UI Object Materials
	Material* ExitButtonMaterial				= new Material(TextureShader, ExitButtonTexture);
	Material* SelectExitMaterial				= new Material(TextureShader, SelectExitTexture);
	Material* OptionsButtonMaterial				= new Material(TextureShader, OptionsButtonTexture);
	Material* SelectOptionMaterial				= new Material(TextureShader, SelectOptionsTexture);
	Material* PlayButtonMaterial				= new Material(TextureShader, PlayButtonTexture);
	Material* SelectPlayMaterial				= new Material(TextureShader, SelectPlayTexture);
	Material* PressEnterMaterial				= new Material(TextureShader, PressEnterTexture);
	Material* NoMusicMaterial					= new Material(TextureShader, NoMusicTexture);
	Material* SelectNoMusicMaterial				= new Material(TextureShader, SelectNoMusicTexture);
	Material* LudicrousSpeedMaterial			= new Material(TextureShader, LudicrousSpeedTexture);
	Material* SelectLudicrousSpeedMaterial		= new Material(TextureShader, SelectLudicrousSpeedTexture);
	Material* MainMenuMaterial					= new Material(TextureShader, MainMenuTexture);
	Material* SelectMainMenuMaterial			= new Material(TextureShader, SelectMainMenuTexture);
	Material* UIMaterial						= new Material(TextureShader, UITexture);
	Material* ZeroMaterial						= new Material(TextureShader, Zero);
	Material* OneMaterial						= new Material(TextureShader, One);
	Material* TwoMaterial						= new Material(TextureShader, Two);
	Material* ThreeMaterial						= new Material(TextureShader, Three);
	Material* FourMaterial						= new Material(TextureShader, Four);
	Material* FiveMaterial						= new Material(TextureShader, Five);
	Material* SixMaterial						= new Material(TextureShader, Six);
	Material* SevenMaterial						= new Material(TextureShader, Seven);
	Material* EightMaterial						= new Material(TextureShader, Eight);
	Material* NineMaterial						= new Material(TextureShader, Nine);
	Material* AudioButtonMaterial				= new Material(TextureShader, AudioButtonTexture);
	Material* SelectAudioMaterial				= new Material(TextureShader, SelectAudioTexture);
	Material* AudioTitleMaterial				= new Material(TextureShader, AudioTitleTexture);
	Material* MusicSliderMaterial				= new Material(TextureShader, AudioMusicSliderTexture);
	Material* FXSliderMaterial					= new Material(TextureShader, AudioFXSliderTexture);
	Material* SliderNubMaterial					= new Material(TextureShader, SliderNubTexture);
	Material* GameOverTitleMaterial				= new Material(TextureShader, GameOverTitleTexture);
	Material* ContinueButtonMaterial			= new Material(TextureShader, ContinueButtonTexture);
	Material* SelectContinueMaterial			= new Material(TextureShader, SelectContinueTexture);
	Material* PauseTitleMaterial				= new Material(TextureShader, PauseTitleTexture);
	Material* ScoreTitleMaterial				= new Material(TextureShader, ScoreTitleTexture);
	Material* GoMaterial						= new Material(TextureShader, GoTexture);
	Material* WinMaterial						= new Material(TextureShader, WinTexture);

	//Render to Texture Material
	Material* RenderMaterial					= new Material(PostRenderShader, RenderTexture);

	//Debug Draw Material
	Material* DebugMaterial						= new Material(DebugDrawShader, MyColor(0, 255, 200, 255));
	

	
	//Create all the meshes
	Mesh* CameraMesh							= new Mesh();
	Mesh* RenderMesh							= new Mesh();
	Mesh* UIMesh								= new Mesh();
	Mesh* UIRectangleMesh						= new Mesh();
	Mesh* UIDigitFrame							= new Mesh();
	Mesh* Cube2									= new Mesh();
	Mesh* Sphere								= new Mesh();
	Mesh* MoonMesh								= new Mesh();
	Mesh* SpaceShip								= new Mesh();
	Mesh* AsteroidMesh							= new Mesh();
	Mesh* UniverseSphere						= new Mesh();
	Mesh* SpaceRing								= new Mesh();
	Mesh* FinishRing							= new Mesh();

	//Initialize their shapes
	UIMesh->									CreateBox(vec2(1.7f, 1.0f), vec2(0.0f, 0.0f));
	UIRectangleMesh->							CreateBox(vec2(7.72f, 1.0f), vec2(0.0f, 0.0f));
	UIDigitFrame->								CreateBox(vec2(1.0f, 0.56f), vec2(0.0f, 0.0f));
	RenderMesh->								CreateBox(vec2(1.0f, 1.0f), vec2(0.0f, 0.0f));
	Cube2->										CreateMeshFromOBJFile("Data/Objs/cube.obj");
	Sphere->									CreateMeshFromOBJFile("Data/Objs/sphere.obj");
	MoonMesh->									CreateMeshFromOBJFile("Data/Objs/Moon.obj");
	SpaceShip->									CreateMeshFromOBJFile("Data/Objs/SpaceShip.obj");
	AsteroidMesh->								CreateMeshFromOBJFile("Data/Objs/Asteroid.obj");
	UniverseSphere->							CreateMeshFromOBJFile("Data/Objs/UniverseSphere.obj");
	SpaceRing->									CreateMeshFromOBJFile("Data/Objs/SpaceRaceRings.obj");
	FinishRing->								CreateMeshFromOBJFile("Data/Objs/FinishLine.obj");

	//Add all resources to Resource Manager

	//Add Shaders
	{
		m_pResources->							AddShader("TextureShader", TextureShader);
		m_pResources->							AddShader("MainShader", MainShader);
		m_pResources->							AddShader("DebugDraw", DebugDrawShader);
		m_pResources->							AddShader("LampShader", LampShader);
		m_pResources->							AddShader("PostRender", PostRenderShader);
		m_pResources->							AddShader("CubemAP", CubeMapShader);
	}
	//Add Textures
	{
		m_pResources->							AddTexture("CubeMap", cubemap);
		m_pResources->							AddTexture("AsteroidTexture", AsteriodTexture);
		m_pResources->							AddTexture("MoonTexture", MoonTexture);
		m_pResources->							AddTexture("ShipTexture", SpaceShipTexture);
		m_pResources->							AddTexture("UniverseTexture", UniverseTexture);
		m_pResources->							AddTexture("VenusTexture", VenusTexture);
		m_pResources->							AddTexture("CeresTexture", CeresTexture);
		m_pResources->							AddTexture("ErisTexture", ErisTexture);
		m_pResources->							AddTexture("HaumealTexture", HaumealTexture);
		m_pResources->							AddTexture("MakemakeTexture", MakemakeTexture);
		m_pResources->							AddTexture("MarsTexture", MarsTexture);
		m_pResources->							AddTexture("NeptuneTexture", NeptuneTexture);
		m_pResources->							AddTexture("SunTexture", SunTexture);
		m_pResources->							AddTexture("FinishLineTexture", FinishLineTexture);
		m_pResources->							AddTexture("RingTexture", RingTexture);
		m_pResources->							AddTexture("0", Zero);
		m_pResources->							AddTexture("1", One);
		m_pResources->							AddTexture("2", Two);
		m_pResources->							AddTexture("3", Three);
		m_pResources->							AddTexture("4", Four);
		m_pResources->							AddTexture("5", Five);
		m_pResources->							AddTexture("6", Six);
		m_pResources->							AddTexture("7", Seven);
		m_pResources->							AddTexture("8", Eight);
		m_pResources->							AddTexture("9", Nine);
		m_pResources->							AddTexture("GoTexture", GoTexture);
		m_pResources->							AddTexture("ExitButtonTexture", ExitButtonTexture);
		m_pResources->							AddTexture("SelectExitTexture", SelectExitTexture);
		m_pResources->							AddTexture("OptionsButtonTexture", OptionsButtonTexture);
		m_pResources->							AddTexture("SelectOptionsTexture", SelectOptionsTexture);
		m_pResources->							AddTexture("PlayButtonTexture", PlayButtonTexture);
		m_pResources->							AddTexture("SelectPlayTexture", SelectPlayTexture);
		m_pResources->							AddTexture("PressEnterTexture", PressEnterTexture);
		m_pResources->							AddTexture("NoMusicTexture", NoMusicTexture);
		m_pResources->							AddTexture("SelectNoMusicTexture", SelectNoMusicTexture);
		m_pResources->							AddTexture("LudicrousSpeedTexture", LudicrousSpeedTexture);
		m_pResources->							AddTexture("SelectLudicrousSpeedTexture", SelectLudicrousSpeedTexture);
		m_pResources->							AddTexture("MainMenuTexture", MainMenuTexture);
		m_pResources->							AddTexture("SelectMainMenuTexture", SelectMainMenuTexture);
		m_pResources->							AddTexture("AudioButtonTexture", AudioButtonTexture);
		m_pResources->							AddTexture("SelectAudioTexture", SelectAudioTexture);
		m_pResources->							AddTexture("AudioTitleTexture", AudioTitleTexture);
		m_pResources->							AddTexture("AudioMusicSliderTexture", AudioMusicSliderTexture);
		m_pResources->							AddTexture("AudioFXSliderTexture", AudioFXSliderTexture);
		m_pResources->							AddTexture("SliderNubTexture", SliderNubTexture);
		m_pResources->							AddTexture("GameOverTitle", GameOverTitleTexture);
		m_pResources->							AddTexture("ContinueButton", ContinueButtonTexture);
		m_pResources->							AddTexture("SelectContinue", SelectContinueTexture);
		m_pResources->							AddTexture("PauseTitle", PauseTitleTexture);
		m_pResources->							AddTexture("ScoreTitle", ScoreTitleTexture);
		m_pResources->							AddTexture("WinTitle", WinTexture);
		m_pResources->							AddTexture("UITexture", UITexture);
		m_pResources->							AddTexture("RenderTexture", RenderTexture);
	}
	//Add Materials
	{
		m_pResources->							AddMaterial("CubeMapMat", CubeMapMaterial);
		m_pResources->							AddMaterial("Camera", CamMat);
		m_pResources->							AddMaterial("DebugMaterial", DebugMaterial);
		m_pResources->							AddMaterial("WhiteLightMaterial", WhiteLampMaterial);
		m_pResources->							AddMaterial("RedLightMaterial", RedLampMaterial);
		m_pResources->							AddMaterial("GreenLightMaterial", GreenLampMaterial);
		m_pResources->							AddMaterial("BlueLightMaterial", BlueLampMaterial);
		m_pResources->							AddMaterial("MoonMaterial", MoonMaterial);
		m_pResources->							AddMaterial("ShipMaterial", ShipMaterial);
		m_pResources->							AddMaterial("AsteroidMaterial", AsteroidMaterial);
		m_pResources->							AddMaterial("PlanetoidMat0", VenusMaterial);
		m_pResources->							AddMaterial("PlanetoidMat1", CeresMaterial);
		m_pResources->							AddMaterial("PlanetoidMat2", ErisMaterial);
		m_pResources->							AddMaterial("PlanetoidMat3", HaumealMaterial);
		m_pResources->							AddMaterial("PlanetoidMat4", MakemakeMaterial);
		m_pResources->							AddMaterial("PlanetoidMat5", MarsMaterial);
		m_pResources->							AddMaterial("PlanetoidMat6", NeptuneMaterial);
		m_pResources->							AddMaterial("PlanetoidMat7", SunMaterial);
		m_pResources->							AddMaterial("RingMaterial", RingMaterial);
		m_pResources->							AddMaterial("FinishLineMaterial", FinishLineMaterial);
		m_pResources->							AddMaterial("UniverseMaterial", UniverseMaterial);
		m_pResources->							AddMaterial("ExitButtonMaterial", ExitButtonMaterial);
		m_pResources->							AddMaterial("SelectExitMaterial", SelectExitMaterial);
		m_pResources->							AddMaterial("OptionsButtonMaterial", OptionsButtonMaterial);
		m_pResources->							AddMaterial("SelectOptionMaterial", SelectOptionMaterial);
		m_pResources->							AddMaterial("PlayButtonMaterial", PlayButtonMaterial);
		m_pResources->							AddMaterial("SelectPlayMaterial", SelectPlayMaterial);
		m_pResources->							AddMaterial("PressEnterMaterial", PressEnterMaterial);
		m_pResources->							AddMaterial("NoMusicMaterial", NoMusicMaterial);
		m_pResources->							AddMaterial("SelectNoMusicMaterial", SelectNoMusicMaterial);
		m_pResources->							AddMaterial("LudicrousSpeedMaterial", LudicrousSpeedMaterial);
		m_pResources->							AddMaterial("SelectLudicrousSpeedMaterial", SelectLudicrousSpeedMaterial);
		m_pResources->							AddMaterial("MainMenuMaterial", MainMenuMaterial);
		m_pResources->							AddMaterial("SelectMainMenuMaterial", SelectMainMenuMaterial);
		m_pResources->							AddMaterial("UIMaterial", UIMaterial);
		m_pResources->							AddMaterial("0", ZeroMaterial);
		m_pResources->							AddMaterial("1", OneMaterial);
		m_pResources->							AddMaterial("2", TwoMaterial);
		m_pResources->							AddMaterial("3", ThreeMaterial);
		m_pResources->							AddMaterial("4", FourMaterial);
		m_pResources->							AddMaterial("5", FiveMaterial);
		m_pResources->							AddMaterial("6", SixMaterial);
		m_pResources->							AddMaterial("7", SevenMaterial);
		m_pResources->							AddMaterial("8", EightMaterial);
		m_pResources->							AddMaterial("9", NineMaterial);
		m_pResources->							AddMaterial("Go", GoMaterial);
		m_pResources->							AddMaterial("AudioButtonMaterial", AudioButtonMaterial);
		m_pResources->							AddMaterial("SelectAudioMaterial", SelectAudioMaterial);
		m_pResources->							AddMaterial("AudioTitleMaterial", AudioTitleMaterial);
		m_pResources->							AddMaterial("MusicSliderMaterial", MusicSliderMaterial);
		m_pResources->							AddMaterial("FXSliderMaterial", FXSliderMaterial);
		m_pResources->							AddMaterial("SliderNubMaterial", SliderNubMaterial);
		m_pResources->							AddMaterial("GameOverMaterial", GameOverTitleMaterial);
		m_pResources->							AddMaterial("ContinueButtonMaterial", ContinueButtonMaterial);
		m_pResources->							AddMaterial("SelectContinueMaterial", SelectContinueMaterial);
		m_pResources->							AddMaterial("PauseTitleMaterial", PauseTitleMaterial);
		m_pResources->							AddMaterial("ScoreTitleMaterial", ScoreTitleMaterial);
		m_pResources->							AddMaterial("WinMaterial", WinMaterial);
		m_pResources->							AddMaterial("RenderMaterial", RenderMaterial);
	}
	//Add Meshes
	{
		m_pResources->							AddMesh("CameraMesh", CameraMesh);
		m_pResources->							AddMesh("Sphere", Sphere);
		m_pResources->							AddMesh("CubeMesh2", Cube2);
		m_pResources->							AddMesh("MoonMesh", MoonMesh);
		m_pResources->							AddMesh("ShipMesh", SpaceShip);
		m_pResources->							AddMesh("AsteroidMesh", AsteroidMesh);
		m_pResources->							AddMesh("UniverseMesh", UniverseSphere);
		m_pResources->							AddMesh("UIMesh", UIMesh);
		m_pResources->							AddMesh("UIRectangleMesh", UIRectangleMesh);
		m_pResources->							AddMesh("UIDigitFrame", UIDigitFrame);
		m_pResources->							AddMesh("RingMesh", SpaceRing);
		m_pResources->							AddMesh("FinishLineMesh", FinishRing);
		m_pResources->							AddMesh("RenderMesh", RenderMesh);
	}
	
	//Create the Music Objects
	SoundPlayer* myMusicPlayer					= m_pAudio->GetSoundPlayer(PlayerType::PT_MUSIC);

	SoundObject* Background1					= myMusicPlayer->LoadSound("Data/Sounds/BackGround1.wav", FXType::FX_MUSIC);
	SoundObject* Background2					= myMusicPlayer->LoadSound("Data/Sounds/BackGround2.wav", FXType::FX_MUSIC);
	SoundObject* Background3					= myMusicPlayer->LoadSound("Data/Sounds/BackGround3.wav", FXType::FX_MUSIC);
	SoundObject* Background4					= myMusicPlayer->LoadSound("Data/Sounds/BackGround4.wav", FXType::FX_MUSIC);
	SoundObject* Background5					= myMusicPlayer->LoadSound("Data/Sounds/BackGround5.wav", FXType::FX_MUSIC);
	SoundObject* Background6					= myMusicPlayer->LoadSound("Data/Sounds/BackGround6.wav", FXType::FX_MUSIC);
	SoundObject* Background7					= myMusicPlayer->LoadSound("Data/Sounds/BackGround7.wav", FXType::FX_MUSIC);
	SoundObject* Background8					= myMusicPlayer->LoadSound("Data/Sounds/BackGround8.wav", FXType::FX_MUSIC);
	SoundObject* Background9					= myMusicPlayer->LoadSound("Data/Sounds/BackGround9.wav", FXType::FX_MUSIC);
	SoundObject* UIBackground					= myMusicPlayer->LoadSound("Data/Sounds/UIBackground3.wav", FXType::FX_MUSIC);

	//Add Music clips to Audio controller
	{
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround1, Background1);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround2, Background2);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround3, Background3);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround4, Background4);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround5, Background5);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround6, Background6);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround7, Background7);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround8, Background8);
		m_pAudio->								AddMusicClip(MusicClipOrder::BackGround9, Background9);
		m_pAudio->								AddMusicClip(MusicClipOrder::UIBackGround, UIBackground);
	}

	//Create FX Sound Objects
	SoundPlayer* myEffectsPlayer				= m_pAudio->GetSoundPlayer(PlayerType::PT_MISC);

	SoundObject* FX_Accelerate					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Accelerate.wav", FXType::FX_CONST);
	SoundObject* FX_Deccelerate					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Deccelerate.wav", FXType::FX_CONST);
	SoundObject* FX_TopSpeed					= myEffectsPlayer->LoadSound("Data/Sounds/FX_TopSpeed.wav", FXType::FX_CONST);
	SoundObject* FX_Idle						= myEffectsPlayer->LoadSound("Data/Sounds/FX_Idle.wav", FXType::FX_CONST);
	SoundObject* FX_Boost						= myEffectsPlayer->LoadSound("Data/Sounds/FX_Boost.wav", FXType::FX_TEMP);
	SoundObject* FX_Announcer_1					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Announcer_1.wav", FXType::FX_TEMP);
	SoundObject* FX_Announcer_2					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Announcer_2.wav", FXType::FX_TEMP);
	SoundObject* FX_Announcer_3					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Announcer_3.wav", FXType::FX_TEMP);
	SoundObject* FX_Announcer_Go				= myEffectsPlayer->LoadSound("Data/Sounds/FX_Announcer_Go.wav", FXType::FX_TEMP);
	SoundObject* FX_Announcer_Failure			= myEffectsPlayer->LoadSound("Data/Sounds/FX_Announcer_Failure.wav", FXType::FX_TEMP);
	SoundObject* FX_Announcer_Success			= myEffectsPlayer->LoadSound("Data/Sounds/FX_Announcer_Success.wav", FXType::FX_TEMP);
	SoundObject* FX_Lasers_1					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Lasers_1.wav", FXType::FX_TEMP);
	SoundObject* FX_Lasers_2					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Lasers_2.wav", FXType::FX_TEMP);
	SoundObject* FX_Lasers_3					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Lasers_3.wav", FXType::FX_TEMP);
	SoundObject* FX_Lasers_4					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Lasers_4.wav", FXType::FX_TEMP);
	SoundObject* FX_Lasers_5					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Lasers_5.wav", FXType::FX_TEMP);
	SoundObject* FX_MouseOverSound				= myEffectsPlayer->LoadSound("Data/Sounds/FX_MouseOverSound.wav", FXType::FX_TEMP);
	SoundObject* FX_PlayButtonSound				= myEffectsPlayer->LoadSound("Data/Sounds/FX_PlayButtonSound.wav", FXType::FX_TEMP);
	SoundObject* FX_ClickButtonSound			= myEffectsPlayer->LoadSound("Data/Sounds/FX_ClickButtonSound.wav", FXType::FX_TEMP);
	SoundObject* FX_Explosion1					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Explosion1.wav", FXType::FX_TEMP);
	SoundObject* FX_Explosion2					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Explosion2.wav", FXType::FX_TEMP);
	SoundObject* FX_Explosion3					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Explosion3.wav", FXType::FX_TEMP);
	SoundObject* FX_Explosion4					= myEffectsPlayer->LoadSound("Data/Sounds/FX_Explosion4.wav", FXType::FX_TEMP);

	//Add FXSounds to AudioController
	{
		m_pAudio->								AddSoundFX(FX_Accelerate, Sounds::Accelarate);
		m_pAudio->								AddSoundFX(FX_Deccelerate, Sounds::Deccelarate);
		m_pAudio->								AddSoundFX(FX_Idle, Sounds::Idle);
		m_pAudio->								AddSoundFX(FX_Boost, Sounds::Boost);
		m_pAudio->								AddSoundFX(FX_TopSpeed, Sounds::TopSpeed);
		m_pAudio->								AddSoundFX(FX_Announcer_1, Sounds::Announcer_1);
		m_pAudio->								AddSoundFX(FX_Announcer_2, Sounds::Announcer_2);
		m_pAudio->								AddSoundFX(FX_Announcer_3, Sounds::Announcer_3);
		m_pAudio->								AddSoundFX(FX_Announcer_Go, Sounds::Announcer_Go);
		m_pAudio->								AddSoundFX(FX_Announcer_Failure, Sounds::Announcer_Failure);
		m_pAudio->								AddSoundFX(FX_Announcer_Success, Sounds::Announcer_Success);
		m_pAudio->								AddSoundFX(FX_Lasers_1, Sounds::Lasers_1);
		m_pAudio->								AddSoundFX(FX_Lasers_2, Sounds::Lasers_2);
		m_pAudio->								AddSoundFX(FX_Lasers_3, Sounds::Lasers_3);
		m_pAudio->								AddSoundFX(FX_Lasers_4, Sounds::Lasers_4);
		m_pAudio->								AddSoundFX(FX_Lasers_5, Sounds::Lasers_5);
		m_pAudio->								AddSoundFX(FX_Explosion1, Sounds::Explosion_1);
		m_pAudio->								AddSoundFX(FX_Explosion2, Sounds::Explosion_2);
		m_pAudio->								AddSoundFX(FX_Explosion3, Sounds::Explosion_3);
		m_pAudio->								AddSoundFX(FX_Explosion4, Sounds::Explosion_4);
		m_pAudio->								AddSoundFX(FX_MouseOverSound, Sounds::MouseOver);
		m_pAudio->								AddSoundFX(FX_PlayButtonSound, Sounds::PlayButton);
		m_pAudio->								AddSoundFX(FX_ClickButtonSound, Sounds::ClickButton);
	}


	//Create PlayerControllers
	for (int i = 0; i < 4; i++)
	{
		PlayerController* Controller = new PlayerController();
		m_pResources->AddController(Controller);
	}

}
