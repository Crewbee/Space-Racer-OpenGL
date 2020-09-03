#pragma once
class ResourceManager;
class Material;
class Texture;
class Mesh;
class ShaderProgram;
class AudioController;
class SoundPlayer;
class SoundObject;
class FBO;

class ResourceFactory
{
protected:

	ResourceManager* m_pResources;
	AudioController* m_pAudio;
    FBO* m_FBO;

public:
	ResourceFactory(ResourceManager* aResourceManager, AudioController* aAudioController, FBO* aFBO);
	~ResourceFactory();

	void CreateResources();

private:

};