#pragma once
#include "Mesh/Material.h"
#include "Mesh/Mesh.h"
#include "Mesh/Texture.h"
#include "XAudio2/SoundObject.h"
#include "XAudio2/SoundPlayer.h"

class Game;
class ShaderProgram;
class PlayerController;

class ResourceManager
{
protected:
    std::map<std::string, Mesh*> m_pMeshes;
    std::map<std::string, ShaderProgram*> m_pShaders;
    std::map<std::string, Texture*> m_pTextures;
    std::map<std::string, Material*> m_pMaterials;

	std::vector<PlayerController*>m_pMyControllers;

	int m_ControllerIndex;

public:
    ResourceManager();
    virtual ~ResourceManager();

    Mesh* AddMesh(std::string name, Mesh* pMesh);
    void AddShader(std::string name, ShaderProgram* pShader);
    void AddTexture(std::string name, Texture* pTexture);
    void AddMaterial(std::string name, Material* pMaterial);
	void AddController(PlayerController* aController);

    Mesh*				GetMesh(const std::string name);
    ShaderProgram*		GetShader(const std::string name);
    Texture*			GetTexture(const std::string name);
    Material*			GetMaterial(const std::string name);
	PlayerController*	GetControllerAt(unsigned int anIndex);
	PlayerController*	GetPlayerController(bool IsNewPlayer);
};

